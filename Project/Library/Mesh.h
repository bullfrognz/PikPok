
//
//  File Name   :   Mesh.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Mesh_H__
#define __Mesh_H__

// Dependent Includes
#include "Library/Renderable.h"

// Local Includes
#include "Library/Vector3.h"
#include "Library/Colour.h"
#include "Library/Material.h"
#include "Defines/Types.h"

// Library Includes
#include <vector>

// Prototypes
class CEntity;
class IVertexBuffer;
class IIndexBuffer;
class CModelLoader;
struct TVector3;

class CMesh : public CRenderable
{

	// Member Types
public:

	enum EAttribrute
	{
		INVALID_ATTRIBRUTE,

		ATTRIBRUTE_NORMALS,
		ATTRIBRUTE_TEXTURE_CORDS,

		MAX_ATTRIBRUTES
	};

	struct TSubset
	{
		TMaterial	tMaterial;

		uint		uiIndicesOffset;
		uint		uiIndicesCount;
	};

	// Member Functions
public:

			 CMesh(CEntity& _rParent);
	virtual ~CMesh();

	virtual void Render();

	bool LoadModel(c_char* _kcpModelAlias);

	uint	 SubsetsCount();
	TSubset& Subset(uint _uiIndex);

	bool HasAttribrute(EAttribrute _eAttribrute);

protected:

	uint ComputeStride();

	bool LoadAttribrutes();
	bool LoadVertexBuffer();
	bool LoadIndexBuffer();
	bool LoadSubsets();

private:

	CMesh(const CMesh& _krMesh);
	CMesh& operator = (const CMesh& _krMesh);

	// Member Variables
protected:

private:

	CEntity&		m_rParent;

	CModelLoader*	m_pModelLoader;

	IVertexBuffer*	m_pVertexBuffer;
	IIndexBuffer*	m_pIndexBuffer;
	TSubset*	m_pSubsets;

	uint			m_uiSubsetsCount;
	uint			m_uiSupportAttribrutes;
	uint			m_uiStride;

};

#endif //__Mesh_H__