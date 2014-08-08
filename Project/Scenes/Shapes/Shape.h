
//
//  File Name   :   Shape.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Shape_H__
#define __Shape_H__

// Dependent Includes
#include "Library/Entity.h"

// Local Includes
#include "Scenes/Shapes/PathMover.h"
#include "Library/Colour.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes
class CMesh;

class CShape : public CEntity
{

	// Member Types
public:

	enum EType
	{
		INVALID_TYPE,

		TYPE_SQUARE,
		TYPE_TRIANGLE,
		TYPE_CIRCLE,
		TYPE_HEXAGON,

		MAX_TYPE
	};

	// Member Functions
public:

			 CShape();
	virtual ~CShape();

	bool Initialise(c_char* _kcpModelFile, const TColour& _ktrColour, const TVector3& _ktrStartPosition, CPathMover::EPattern _eMovePattern, float _fDimention, float _fMoveSpeed);

	void Colour(const TColour& _ktrColour);
	void Dimention(float _fDimention);

	CPathMover&	 PathMover();
	EType		 Type()			const;
	float		 Dimention()	const;

protected:

private:

	void Cleanup();

	CShape(const CShape& _krShape);
	CShape& operator = (const CShape& _krShape);

	// Member Variables
protected:

	TColour			m_tColour;

	CPathMover*		m_pPathMover;
	CMesh*			m_pMesh;

	EType			m_eType;

	float			m_fDimention;

private:

};

#endif //__Shape_H__