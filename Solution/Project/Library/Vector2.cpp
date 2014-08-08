
//
//  File Name   :   Vector2.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Vector2.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

TVector2::TVector2()
: x(0.0f)
, y(0.0f)
{
	// Empty
}

TVector2::TVector2(float _fX, float _fY)
: x(_fX)
, y(_fY)
{
}

TVector2::TVector2(const TVector2& _krRight)
: x(_krRight.x)
, y(_krRight.y)
{
	// Empty
}

TVector2::~TVector2()
{
	// Empty
}

float TVector2::Magnitude() const
{
	float fMagnitudeSquare = x * x;
	fMagnitudeSquare += y * y;

	return ((float)sqrt(fMagnitudeSquare));
}

float TVector2::MagnitudeSquare() const
{
	float fMagnitudeSquare = x * x;
	fMagnitudeSquare += y * y;

	return (fMagnitudeSquare);
}

float TVector2::Distance(const TVector2& _kvrTarget) const
{
	TVector2 vDifference = (*this) - _kvrTarget;

	return (vDifference.Magnitude());
}

float TVector2::DistanceSquare(const TVector2& _kvrTarget) const
{
	TVector2 vDifference = (*this) - _kvrTarget;

	return (vDifference.MagnitudeSquare());
}

void TVector2::Normalise()
{
	float fMagnitude = (float)sqrt(x*x + y*y);

	if (fMagnitude)
	{
		x /= fMagnitude;
		y /= fMagnitude;
	}
}

void TVector2::Set(float _fX, float _fY)
{
	x = _fX;
	y = _fY;
}

const TVector2& TVector2::operator += (const TVector2& _krRight)
{
	x += _krRight.x;
	y += _krRight.y;

	return (*this);
}

const TVector2& TVector2::operator -= (const TVector2& _krRight)
{
	x -= _krRight.x;
	y -= _krRight.y;

	return (*this);
}

const TVector2& TVector2::operator *= (float _fValue)
{
	x *= _fValue;
	y *= _fValue;

	return (*this);
}

const TVector2& TVector2::operator /= (float _fValue)
{
	x /= _fValue;
	y /= _fValue;

	return (*this);
}

TVector2 TVector2::operator + (const TVector2& _krRight) const
{
	TVector2 TempVector(x + _krRight.x,
						y + _krRight.y);

	return (TempVector);
}

TVector2 TVector2::operator - (const TVector2& _krRight) const
{
	TVector2 TempVector(x - _krRight.y,
						y - _krRight.x);

	return (TempVector);
}

TVector2 TVector2::operator * (float _fValue) const
{
	TVector2 TempVector(x * _fValue,
						y * _fValue);

	return (TempVector);
}

TVector2 TVector2::operator / (float _fValue) const
{
	TVector2 TempVector(x / _fValue,
						y / _fValue);

	return (TempVector);
}

float& TVector2::operator [] (uchar _ucIndex)
{
	switch (_ucIndex)
	{
	case 0: return (x);
	case 1: return (y);

	default:
		DEBUG_ERROR("Invalid index for TVector2 container. Index(%d)", _ucIndex);
		break;
	}

	static float fInvalid = -1.0f;
	return (fInvalid);
}

bool TVector2::operator == (const TVector2&_krRight) const
{
	if (x == _krRight.x &&
		y == _krRight.y)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool TVector2::operator != (const TVector2&_krRight) const
{
	if (x != _krRight.x ||
		y != _krRight.y)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}