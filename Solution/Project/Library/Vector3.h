
//
//  File Name   :   Vector3.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Vector3_H__
#define __Vector3_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TVector4;

struct TVector3
{

// Member Types

// Member Functions

	 TVector3();
	 TVector3(float _fX, float _fY, float _fZ);
	 TVector3(const TVector3& _krRight);
	 TVector3(const TVector4& _krRight);
	~TVector3();

	float		Magnitude() const;
	float		MagnitudeSquared() const;

	float		Dot(const TVector3& _krVector3) const;
	float		DotAngle(const TVector3& _krVector3) const;

	TVector3&	Normalise();
	TVector3	Lerp(const TVector3& _krTo, float _fTime);

	TVector3	DisplacementTo(const TVector3& _kvrTarget) const;
	TVector3&	DisplacementTo(const TVector3& _kvrTarget, TVector3& _vrResult) const;
	float		DistanceTo(const TVector3& _kvrTarget) const;
	float		DistanceToSquared(const TVector3& _kvrTarget) const;

	void		Set(float _fX, float _fY, float _fZ);

	bool		IsZero();

	TVector3& operator += (const TVector3& _krRight);
	TVector3& operator -= (const TVector3& _krRight);
	TVector3& operator *= (float _fValue);
	TVector3& operator /= (float _fValue);

	TVector3 operator + (const TVector3& _krRight) const;
	TVector3 operator - (const TVector3& _krRight) const;
	TVector3 operator * (float _fValue) const;
	TVector3 operator / (float _fValue) const;

	float& operator []	(uchar _ucIndex);

	bool operator == (const TVector3& _krRight) const;
	bool operator != (const TVector3& _krRight) const;

// Member Variables

	float x;
	float y;
	float z;

	static TVector3 zero;

};

#endif //__Vector3_H__