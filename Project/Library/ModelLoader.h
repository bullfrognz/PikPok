
//
//  File Name   :   ModelLoader.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __ModelLoader_H__
#define __ModelLoader_H__

// Dependent Includes

// Local Includes
#include "Library/List.h"
#include "Defines/Types.h"

// Library Includes
#include <vector>

// Prototypes
class IShader;
struct TMaterial;
struct TVector2;
struct TVector3;
struct aiMesh;
struct aiScene;
struct aiMaterial;

class CModelLoader
{

	// Member Types
public:

	struct TSubset
	{
		char* pName;
		uint uiNumIndices;
		uint uiIndexOffset;
		uint uiMaterialId;
	};

	// Member Functions
public:

			 CModelLoader();
	virtual ~CModelLoader();

	bool Load(c_char* _kcpModelAlias);

	CList<TSubset>&		Subsets();
	CList<TMaterial>&	Materials();
	CList<TVector3>&	Vertices();
	CList<TVector3>&	Normals();
	CList<TVector2>&	TextureCoords();
	CList<uint>&		Indices();
	CList<uint>&		AttribruteTable();

protected:

	uint ComputeStride();

private:

	bool ExtractModelMaterials(const aiScene* _ktpScene);
	bool ExtractModelSubsets(const aiScene* _ktpScene);

	bool ExtractSubsetVertices(const aiMesh* _tpMesh);
	bool ExtractSubsetIndices(const aiMesh* _tpMesh, uint _uiStartNumVertices);

	bool AddSubset(const aiMesh* _ktpMesh);

	CModelLoader(const CModelLoader& _krModelLoader);
	CModelLoader& operator = (const CModelLoader& _krModelLoader);

	// Member Variables
protected:

private:

	char* m_cpFilePath;

	CList<TSubset>		m_aSubsets;
	CList<TMaterial>	m_aMaterials;
	CList<std::string>	m_aShaderAliases;
	CList<TVector3>		m_aVertices;
	CList<TVector3>		m_aNormals;
	CList<TVector2>		m_aTextureCoords;
	CList<uint>			m_aIndices;
	CList<uint>			m_aAttribruteTable;

};

#endif //__ModelLoader_H__