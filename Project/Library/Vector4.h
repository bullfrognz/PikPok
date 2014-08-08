
//
//  File Name   :   Vector4.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Vector4_H__
#define __Vector4_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TVector2;
struct TVector3;
struct TMatrix;

struct TVector4
{

// Member Types

// Member Functions

	 TVector4();
	 TVector4(const TVector2& _krVector);
	 TVector4(const TVector3& _krVector);
	 TVector4(const TVector4& _krVector);
	 TVector4(float _fX, float _fY, float _fZ, float _fW);
	~TVector4();

	void Set(float _fX, float _fY, float _fZ, float _fW);

	TVector4& operator *= (const TMatrix& _kxrMatrix);
	TVector4  operator *  (const TMatrix& _kxrMatrix);

// Member Variables

	float x;
	float y;
	float z;
	float w;

};

#endif //__Vector4_H__