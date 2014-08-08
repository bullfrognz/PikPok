
//
//  File Name   :   AABox.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __AABox_H__
#define __AABox_H__

// Dependent Includes

// Local Includes
#include "Library/Vector3.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes

struct TAABox
{

// Member Types

	enum ESide
	{
		INVALID_SIDE = -1,

		SIDE_TOP,
		SIDE_BOTTOM,
		SIDE_LEFT,
		SIDE_RIGHT,
		SIDE_FRONT,
		SIDE_BACK,

		MAX_SIDE
	};

// Member Functions

			 TAABox();
	virtual ~TAABox();

	void SetBounds(const TVector3& _vCenterPosition, float _fHalfWidth, float _fHalfHeight);

	bool IsWithinBounds(TVector3& _vrPosition) const;
	bool IsWithinBounds(float _fX, float _fY, float _fZ) const;

// Member Variables

	TVector3 m_vCenter;

	float m_fHalfWidth;
	float m_fSideOffset[MAX_SIDE];

};

#endif //__AABox_H__