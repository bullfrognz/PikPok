
//
//  File Name   :   Shape.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Shape.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/Mesh.h"
#include "Library/Shader.h"

// Library Includes6

// Static Initialisers

// Implementation

CShape::CShape()
: m_pPathMover(null)
, m_pMesh(null)
, m_eType(INVALID_TYPE)
, m_fDimention(0)
{
	// Empty
}

CShape::~CShape()
{
	Cleanup();
}

bool CShape::Initialise(c_char* _kcpModelFile, const TColour& _ktrColour, const TVector3& _ktrStartPosition, CPathMover::EPattern _eMovePattern, float _fDimention, float _fMoveSpeed)
{
	Cleanup();

	m_pPathMover = new CPathMover(*this);
	m_pMesh = new CMesh(*this);

	// Load shape mesh
	VALIDATE(m_pMesh);
	VALIDATE(m_pMesh->LoadModel(_kcpModelFile));

	// Set path mover settings
	m_pPathMover->CenterPosition(_ktrStartPosition);
	m_pPathMover->Pattern(_eMovePattern);
	m_pPathMover->MoveSpeed(_fMoveSpeed);
	m_pPathMover->Radius(2.0f);

	Colour(_ktrColour);
	Dimention(_fDimention);

	return (true);
}

void CShape::Colour(const TColour& _ktrColour)
{
	m_tColour = _ktrColour;

	m_pMesh->Subset(0).tMaterial.pShader->SetVariable("g_fColour", m_tColour);
}

void CShape::Dimention(float _fDimention)
{
	m_fDimention = _fDimention;

	CEntity::World().Scale(m_fDimention);
}

CPathMover&	CShape::PathMover()
{
	return (*m_pPathMover);
}

CShape::EType CShape::Type() const
{
	return (m_eType);
}

float CShape::Dimention() const
{
	return (m_fDimention);
}

void CShape::Cleanup()
{
	FW_DELETE(m_pPathMover);
	FW_DELETE(m_pMesh);
}