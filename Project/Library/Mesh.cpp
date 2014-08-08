
//
//  File Name   :   Mesh.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Mesh.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Renderer.h"
#include "Library/VertexBuffer.h"
#include "Library/IndexBuffer.h"
#include "Library/ModelLoader.h"
#include "Library/Vector2.h"
#include "Library/Entity.h"

// Library Includes

// Static Initialisers

// Implementation

CMesh::CMesh(CEntity& _rParent)
: m_rParent(_rParent)
, m_pModelLoader(null)
, m_pVertexBuffer(null)
, m_pIndexBuffer(null)
, m_pSubsets(null)
, m_uiSubsetsCount(0)
, m_uiSupportAttribrutes(0)
, m_uiStride(0)
{
}

CMesh::~CMesh()
{
	FW_DELETE(m_pModelLoader);
	FW_DELETE(m_pVertexBuffer);
	FW_DELETE(m_pIndexBuffer);

	FW_ADELETE(m_pSubsets);
}

void CMesh::Render()
{
	for (uint i = 0; i < m_uiSubsetsCount; ++ i)
	{
		TSubset& trMeshSubset = m_pSubsets[i];

		m_pIndexBuffer->Draw(m_rParent.World(), trMeshSubset.tMaterial, m_pVertexBuffer, trMeshSubset.uiIndicesCount, trMeshSubset.uiIndicesOffset, 0);
	}
}

bool CMesh::LoadModel(c_char* _kcpModelAlias)
{
	m_pModelLoader = new CModelLoader();
	m_pModelLoader->Load(_kcpModelAlias);

	VALIDATE(LoadAttribrutes());
	VALIDATE(LoadVertexBuffer());
	VALIDATE(LoadIndexBuffer());
	VALIDATE(LoadSubsets());

	return (true);
}

uint CMesh::SubsetsCount()
{
	return (m_uiSubsetsCount);
}
CMesh::TSubset& CMesh::Subset(uint _uiIndex)
{
	return (m_pSubsets[_uiIndex]);
}

bool CMesh::HasAttribrute(EAttribrute _eAttribrute)
{
	return ((m_uiSupportAttribrutes & _eAttribrute) > 0);
}

bool CMesh::LoadAttribrutes()
{
	m_uiSupportAttribrutes	= 0;
	m_uiStride				= sizeof(float) * 3; // Position

	if (m_pModelLoader->Normals().Count() > 0)
	{
		m_uiSupportAttribrutes |= ATTRIBRUTE_NORMALS;
		m_uiStride += sizeof(float) * 3;
	}

	if (m_pModelLoader->TextureCoords().Count() > 0)
	{
		m_uiSupportAttribrutes |= ATTRIBRUTE_TEXTURE_CORDS;
		m_uiStride += sizeof(float) * 2;
	}

	return (true);
}

bool CMesh::LoadVertexBuffer()
{
	// Delete previous vertex buffer
	FW_DELETE(m_pVertexBuffer);

	// Compile vertices's data
	CList<float> aVerticesData;

	for (uint i = 0; i < m_pModelLoader->Vertices().Count(); ++ i)
	{
		// Position
		TVector3 vPosition = m_pModelLoader->Vertices()[i];
		aVerticesData.Add(vPosition.x);
		aVerticesData.Add(vPosition.y);
		aVerticesData.Add(vPosition.z);

		// Normals
		if (HasAttribrute(ATTRIBRUTE_NORMALS))
		{
			TVector3 vNormal = m_pModelLoader->Normals()[i];
			aVerticesData.Add(vNormal.x);
			aVerticesData.Add(vNormal.y);
			aVerticesData.Add(vNormal.z);
		}

		// Texture cords
		if (HasAttribrute(ATTRIBRUTE_TEXTURE_CORDS))
		{
			TVector2 vTexCord = m_pModelLoader->TextureCoords()[i];
			aVerticesData.Add(vTexCord.x);
			aVerticesData.Add(vTexCord.y);
		}
	}
	
	// Create vertex buffer & load vertices data
	IVertexBuffer::TDesc tVBufferDesc(IVertexBuffer::USAGE_STATIC,
									  IVertexBuffer::PRIMITIVETYPE_TRIANGLELIST,
									  aVerticesData.Count() / (m_uiStride / sizeof(float)),
									  m_uiStride, 
									  aVerticesData.GetVector().data());

	m_pVertexBuffer = RENDERER.InstanceVertexBuffer();
	m_pVertexBuffer->Initiailise(tVBufferDesc);

	return (true);
}

bool CMesh::LoadIndexBuffer()
{
	// Delete previous index buffer
	FW_DELETE(m_pIndexBuffer);

	// Create index buffer & load indices data
	IIndexBuffer::TDesc tIBufferDesc(IIndexBuffer::USAGE_STATIC,
									 IIndexBuffer::INDEXSIZE_32,
									 m_pModelLoader->Indices().Count(),
									 m_pModelLoader->Indices().GetVector().data());

	m_pIndexBuffer = RENDERER.InstanceIndexBuffer();
	m_pIndexBuffer->Initiailise(tIBufferDesc);

	return (true);
}

bool CMesh::LoadSubsets()
{
	FW_ADELETE(m_pSubsets);

	m_uiSubsetsCount = m_pModelLoader->Subsets().Count();

	m_pSubsets = new TSubset[m_uiSubsetsCount];

	for (uint  i = 0; i < m_uiSubsetsCount; ++ i)
	{
		CModelLoader::TSubset tSubset = m_pModelLoader->Subsets()[i];

		m_pSubsets[i].uiIndicesOffset	= tSubset.uiIndexOffset;
		m_pSubsets[i].uiIndicesCount	= tSubset.uiNumIndices;
		m_pSubsets[i].tMaterial			= m_pModelLoader->Materials()[tSubset.uiMaterialId];
	}

	return (true);
}