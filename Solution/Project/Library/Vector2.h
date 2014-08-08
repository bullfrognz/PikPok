
//
//  File Name   :   Vector2.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Vector2_H__
#define __Vector2_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

struct TVector2
{

// Member Types

// Member Functions

	 TVector2();
	 TVector2(float _fX, float _fY);
	 TVector2(const TVector2& _krRight);
	~TVector2();

	float Magnitude() const;
	float MagnitudeSquare() const;

	float Distance(const TVector2& _kvrTarget) const;
	float DistanceSquare(const TVector2& _kvrTarget) const;

	void Normalise();

	void Set(float _fX, float _fY);

	const TVector2& operator += (const TVector2& _krRight);
	const TVector2& operator -= (const TVector2& _krRight);
	const TVector2& operator *= (float _fValue);
	const TVector2& operator /= (float _fValue);

	TVector2 operator + (const TVector2& _krRight)	const;
	TVector2 operator - (const TVector2& _krRight)	const;
	TVector2 operator * (float _fValue)				const;
	TVector2 operator / (float _fValue)				const;

	float& operator []	(uchar _ucIndex);

	bool operator == (const TVector2& _krRight) const;
	bool operator != (const TVector2& _krRight) const;

	bool IsZero() const;

// Member Variables

	float x;
	float y;

};

#endif //__Vector2_H__