
//
//  File Name   :   ProjectionMatrix.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __ProjectionMatrix_H__
#define __ProjectionMatrix_H__

// Dependent Includes
#include "Library/Matrix.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TViewMatrix;
struct TVector4;

struct TProjectionMatrix : public TMatrix
{

// Member Types

	enum EFrustumPlane
	{
		INVALID_FRUSTUMPLANE = -1,

		FRUSTUMPLANE_LEFT,
		FRUSTUMPLANE_RIGHT,
		FRUSTUMPLANE_TOP,
		FRUSTUMPLANE_BOTTOM,
		FRUSTUMPLANE_NEAR,
		FRUSTUMPLANE_FAR,

		MAX_FRUSTUMPLANE
	};

// Member Functions

	 TProjectionMatrix();
	~TProjectionMatrix();

	void SetPerspectiveFovLH(float _fFoV, float _fAspectRatio, float _fMin, float _fMax);
	void SetOrthographicFovLH(float _fWidth, float _fHeight, float _fMin, float _fMax);

	void GenerateFrustumPlane(const TViewMatrix& _kxrView, EFrustumPlane _eFrustumPlane, TVector4& _rPlane);

// Member Variables

};

#endif //__ProjectionMatrix_H__