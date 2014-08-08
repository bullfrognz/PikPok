
//
//  File Name   :   ModelLoader.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "ModelLoader.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Renderer.h"
#include "Library/Texture.h"
#include "Library/StrUtilities.h"
#include "Library/Vector2.h"
#include "Library/Vector3.h"
#include "Library/Material.h"
#include "Library/Colour.h"
#include "Library/JsonParser.h"
#include "Library/ShaderLoader.h"
#include "Library/Shader.h"
#include "Defines/Macros.h"

// Library Includes
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <vector>
#include <string>

// Static Initialisers

// Implementation

CModelLoader::CModelLoader()
: m_cpFilePath(0)
{
	// Empty
}

CModelLoader::~CModelLoader()
{
	FW_ADELETE(m_cpFilePath);

	CList<TMaterial>::TItems tMaterialItems = m_aMaterials.Items();

	while (!tMaterialItems.End())
	{
		FW_DELETE(tMaterialItems.Value().pShader);

		CList<ITexture*>::TItems tTextureItems = tMaterialItems.Value().aTextures.Items();

		while (!tTextureItems.End())
		{
			FW_DELETE(tTextureItems.Value());

			++ tTextureItems;
		}

		++ tMaterialItems;
	}
}

bool CModelLoader::Load(c_char* _kcpModelAlias)
{
	CJsonParser JosnParser;

	VALIDATE(JosnParser.ParseFile("Json/Models.json"));

	CJsonParser::TValue tValue = JosnParser.BaseValue();

	if (!tValue.IsType(CJsonParser::VALUE_TYPE_OBJECT))
	{
		DEBUG_ERROR("Was expecting base value type to be object");
		return (false);
	}

	if (!tValue.AsObject().HasKey("Directory"))
	{
		DEBUG_ERROR("Missing 'Directory' key.");
		return (false);
	}

	if (!tValue.AsObject()["Directory"].IsType(CJsonParser::VALUE_TYPE_STRING))
	{
		DEBUG_ERROR("'Directory' key invalid type. Expecting string. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	if (!tValue.AsObject().HasKey("Entries"))
	{
		DEBUG_ERROR("Missing 'Entries' key. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	if (!tValue.AsObject()["Entries"].IsType(CJsonParser::VALUE_TYPE_OBJECT))
	{
		DEBUG_ERROR("'Entries' key invalid type. Expecting object. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	CJsonParser::TValue tDirectory	= tValue.AsObject()["Directory"];
	CJsonParser::TValue tEntries	= tValue.AsObject()["Entries"];

	if (!tEntries.AsObject().HasKey(_kcpModelAlias))
	{
		DEBUG_ERROR("'Entries' does not contain model alias. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	CJsonParser::TValue tModelEntry = tEntries.AsObject()[_kcpModelAlias];

	if (!tModelEntry.IsType(CJsonParser::VALUE_TYPE_OBJECT))
	{
		DEBUG_ERROR("Model entry type is invlaid. Expecting type object. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	if (!tModelEntry.AsObject().HasKey("File"))
	{
		DEBUG_ERROR("Model entry is missing 'File' key. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	if (!tModelEntry.AsObject()["File"].IsType(CJsonParser::VALUE_TYPE_STRING))
	{
		DEBUG_ERROR("Model entry 'File' value type is invalid. Expecting type string. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	if (!tModelEntry.AsObject().HasKey("Shaders"))
	{
		DEBUG_ERROR("Model entry is missing 'Shaders' key. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	if (!tModelEntry.AsObject()["Shaders"].IsType(CJsonParser::VALUE_TYPE_ARRAY))
	{
		DEBUG_ERROR("Model entry 'Shaders' value type is invalid. Expecting type array. ModelAlias(%s)", _kcpModelAlias);
		return (false);
	}

	CJsonParser::TValue tFile		= tModelEntry.AsObject()["File"];
	CJsonParser::TValue tShaders	= tModelEntry.AsObject()["Shaders"];

	uint uiNumShaders = tShaders.AsArray().Count();

	for (uint i = 0; i < uiNumShaders; ++ i)
	{
		if (!tShaders.AsArray()[i].IsType(CJsonParser::VALUE_TYPE_STRING))
		{
			DEBUG_ERROR("Model entry 'Shaders' array element type is invalid. Expecting type string. ModelAlias(%s) ShaderArrayIndex(%d)", _kcpModelAlias, i);
			return (false);
		}

		m_aShaderAliases.Add(tShaders.AsArray()[i].AsString());
	}

	std::string FilePath(tDirectory.AsString());
	FilePath += "/";
	FilePath += tFile.AsString();

	// Copy file path
	StrUtilities::Copy(FilePath.c_str(), m_cpFilePath);

	// Read model file
	Assimp::Importer Importer;
	const aiScene* ktpScene = Importer.ReadFile(m_cpFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_MakeLeftHanded);

	if (ktpScene == 0)
	{
		DEBUG_ERROR("Failed to read model file. FilePath(%s)", FilePath.c_str());
	}

	// Extract materials and subsets from model data
	VALIDATE(ExtractModelMaterials(ktpScene));
	VALIDATE(ExtractModelSubsets(ktpScene));

	return (true);
}

CList<CModelLoader::TSubset>& CModelLoader::Subsets()
{
	return (m_aSubsets);
}

CList<TMaterial>& CModelLoader::Materials()
{
	return (m_aMaterials);
}

CList<TVector3>& CModelLoader::Vertices()
{
	return (m_aVertices);
}

CList<TVector3>& CModelLoader::Normals()
{
	return (m_aNormals);
}

CList<TVector2>& CModelLoader::TextureCoords()
{
	return (m_aTextureCoords);
}

CList<uint>& CModelLoader::Indices()
{
	return (m_aIndices);
}

CList<uint>& CModelLoader::AttribruteTable()
{
	return (m_aAttribruteTable);
}

bool CModelLoader::ExtractModelMaterials(const aiScene* _ktpScene)
{
	if (m_aShaderAliases.Count() != _ktpScene->mNumMaterials)
	{
		DEBUG_ERROR("Model shader count does not match the number of materials on this model. FilePath(%s) NumShadersDefined(%d) NumMaterials", m_cpFilePath, m_aShaderAliases.Count(), _ktpScene->mNumMaterials);
		return (false);
	}

	for (uint i = 0; i < _ktpScene->mNumMaterials; ++i)
	{
		TMaterial tMaterial;

		uint uiNumTextures = _ktpScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);

		if (uiNumTextures > 0)
		{
			// Add material textures
			for (uint i = 0; i < uiNumTextures; ++i)
			{
				aiString AiString;
				_ktpScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &AiString);

				std::string TextureFilePath(  StrUtilities::ExtractDirectoryPath(m_cpFilePath) );
				TextureFilePath += AiString.C_Str();

				static ITexture::TDesc tTextureDesc;
				tTextureDesc.eBindFlag = ITexture::BINDFLAG_SHADERRESOURCE;
				tTextureDesc.eCPUAccess = ITexture::CPUACCESS_NONE;
				tTextureDesc.eFormat = ITexture::FORMAT_R8G8B8A8;
				tTextureDesc.eUsage = ITexture::USAGE_STATIC;
				tTextureDesc.uiNumMipMaps = 1;
				tTextureDesc.kcpFile = TextureFilePath.c_str();

				ITexture* pTexture = RENDERER.InstanceTexture();
				pTexture->Initialise(tTextureDesc);

				tMaterial.aTextures.Add(pTexture);
			}
		}

		// Diffuse
		aiColor3D tDiffuse(0.f, 0.f, 0.f);
		_ktpScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, tDiffuse);

		tMaterial.tDiffuse = TColour(tDiffuse.r, tDiffuse.g, tDiffuse.b, 1.0f);

		// Ambient
		aiColor3D tAmbient(0.f, 0.f, 0.f);
		_ktpScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, tAmbient);

		tMaterial.tAmbient = TColour(tAmbient.r, tAmbient.g, tAmbient.b, 1.0f);

		// Specular
		aiColor3D tSpecular(0.f, 0.f, 0.f);
		_ktpScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, tSpecular);

		tMaterial.tSpecular = TColour(tSpecular.r, tSpecular.g, tSpecular.b, 1.0f);

		// Specular power
		float fPower = 0.0f;
		_ktpScene->mMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, fPower);
		tMaterial.fSpecularPower = fPower;

		// Load material shader
		CShaderLoader ShaderLoader;
		tMaterial.pShader = ShaderLoader.Load(m_aShaderAliases[i].c_str());

		m_aMaterials.Add(tMaterial);
	}

	return (true);
}

bool CModelLoader::ExtractModelSubsets(const aiScene* _ktpScene)
{
	for (uint i = 0; i < _ktpScene->mNumMeshes; ++i)
	{
		AddSubset(_ktpScene->mMeshes[i]);

		for (uint uiFace = 0; uiFace < _ktpScene->mMeshes[i]->mNumFaces; ++uiFace)
		{
			m_aAttribruteTable.Add(i);
		}
	}

	return (true);
}

bool CModelLoader::ExtractSubsetVertices(const aiMesh* _tpMesh)
{
	// Extract vertex info
	for (uint uiVertex = 0; uiVertex < _tpMesh->mNumVertices; ++uiVertex)
	{
		TVector3 Position(_tpMesh->mVertices[uiVertex].x, _tpMesh->mVertices[uiVertex].y, _tpMesh->mVertices[uiVertex].z);
		m_aVertices.Add(Position);

		// Vertex normals
		if (_tpMesh->HasNormals())
		{
			TVector3 Normal(_tpMesh->mNormals[uiVertex].x, _tpMesh->mNormals[uiVertex].y, _tpMesh->mNormals[uiVertex].z);
			m_aNormals.Add(Normal);
		}

		// Vertex texture coordinates
		if (_tpMesh->HasTextureCoords(0))
		{
			TVector2 TextureCoords(_tpMesh->mTextureCoords[0][uiVertex].x, _tpMesh->mTextureCoords[0][uiVertex].y);
			m_aTextureCoords.Add(TextureCoords);
		}
	}

	return (true);
}

bool CModelLoader::ExtractSubsetIndices(const aiMesh* _tpMesh, uint _uiStartNumVertices)
{
	// Extract indices
	for (uint uiFace = 0; uiFace < _tpMesh->mNumFaces; ++uiFace)
	{
		aiFace& trFace = _tpMesh->mFaces[uiFace];

		for (uint uiIndex = 0; uiIndex < trFace.mNumIndices; ++uiIndex)
		{
			uint uiValue = _uiStartNumVertices + trFace.mIndices[uiIndex];
			m_aIndices.Add(uiValue);
		}
	}

	return (true);
}

bool CModelLoader::AddSubset(const aiMesh* _ktpMesh)
{
	uint uiStartIndexCount = m_aIndices.Count();
	uint uiStartVertexCount = m_aVertices.Count();

	ExtractSubsetVertices(_ktpMesh);
	ExtractSubsetIndices(_ktpMesh, uiStartVertexCount);

	// Create subset info
	TSubset tSubset;
	tSubset.pName = 0;
	tSubset.uiNumIndices = m_aIndices.Count() - uiStartIndexCount;
	tSubset.uiIndexOffset = uiStartIndexCount;
	tSubset.uiMaterialId = _ktpMesh->mMaterialIndex;

	// Set subset name
	if (_ktpMesh->mName.length > 0)
	{
		StrUtilities::Copy(_ktpMesh->mName.C_Str(), tSubset.pName);
	}

	m_aSubsets.Add(tSubset);

	return (true);
}