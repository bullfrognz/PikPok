
//
//  File Name   :   Vector3.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Vector3.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/Vector4.h"

// Library Includes
#include <cmath>
#include <DirectX/D3DX10math.h>

// Static Initialisers
TVector3 TVector3::zero;

// Implementation

TVector3::TVector3()
: x(0)
, y(0)
, z(0)
{
	// Empty
}

TVector3::TVector3(const TVector3& _kRight)
: x(_kRight.x)
, y(_kRight.y)
, z(_kRight.z)
{
	// Empty
}

TVector3::TVector3(const TVector4& _krRight)
: x(_krRight.x)
, y(_krRight.y)
, z(_krRight.z)
{
	// Empty
}

TVector3::~TVector3()
{
	// Empty
}

TVector3::TVector3(float _fX, float _fY, float _fZ)
: x(_fX)
, y(_fY)
, z(_fZ)
{
	// Empty
}

float TVector3::Magnitude() const
{
	return (D3DXVec3Length(reinterpret_cast<const D3DXVECTOR3*>(this)));
}

float TVector3::MagnitudeSquared() const
{
	float fMagnitudeSquare   = x * x;
	fMagnitudeSquare		+= y * y;
	fMagnitudeSquare		+= z * z;

	return (fMagnitudeSquare);
}

TVector3&
TVector3::Normalise()
{
	D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(this), reinterpret_cast<const D3DXVECTOR3*>(this));

	return(*this);
}

TVector3 TVector3::Lerp(const TVector3& _krTo, float _fTime)
{
	return (((_krTo - (*this)) * _fTime) + (*this));
}

TVector3 TVector3::DisplacementTo(const TVector3& _kvrTarget) const
{
	TVector3 vDisplacement = (*this) - _kvrTarget;

	return (vDisplacement);
}

TVector3& TVector3::DisplacementTo(const TVector3& _kvrTarget, TVector3& _vrResult) const
{
	_vrResult = (*this) - _kvrTarget;

	return (_vrResult);
}

float TVector3::DistanceTo(const TVector3& _kvrTarget) const
{
	return (DisplacementTo(_kvrTarget).Magnitude());
}

float TVector3::DistanceToSquared(const TVector3& _kvrTarget) const
{
	return (DisplacementTo(_kvrTarget).MagnitudeSquared());
}

void TVector3::Set(float _fX, float _fY, float _fZ)
{
	x = _fX;
	y = _fY;
	z = _fZ;
}

bool TVector3::IsZero()
{
	return (FW_MEMCOMPARE(this, &zero, sizeof(TVector3)) == 0);
}

TVector3& TVector3::operator += (const TVector3& _krRight)
{
	x += _krRight.x;
	y += _krRight.y;
	z += _krRight.z;

	return (*this);
}

TVector3& TVector3::operator -= (const TVector3& _kRight)
{
	x -= _kRight.x;
	y -= _kRight.y;
	z -= _kRight.z;

	return (*this);
}

TVector3& TVector3::operator *= (float _fValue)
{
	x *= _fValue;
	y *= _fValue;
	z *= _fValue;

	return (*this);
}

TVector3& TVector3::operator /= (float _fValue)
{
	x /= _fValue;
	y /= _fValue;
	z /= _fValue;

	return (*this);
}

TVector3 TVector3::operator + (const TVector3& _kRight) const
{
	return (TVector3(x + _kRight.x,
					 y + _kRight.y,
					 z + _kRight.z));
}

TVector3 TVector3::operator - (const TVector3& _kRight) const
{
	return (TVector3(x - _kRight.x,
					 y - _kRight.y,
				 	 z - _kRight.z));
}

TVector3 TVector3::operator * (float _fValue) const
{
	return (TVector3(x * _fValue,
					 y * _fValue,
					 z * _fValue));
}

TVector3 TVector3::operator / (float _fValue) const
{
	return (TVector3(x / _fValue,
					 y / _fValue,
					 z / _fValue));
}

float& TVector3::operator [] (uchar _ucIndex)
{
	switch (_ucIndex)
	{
	case 0: return (x);
	case 1: return (y);
	case 2: return (z);

	default:
		DEBUG_ERROR("Invalid index for TVector3 container. Index(%d)", _ucIndex);
		break;
	}

	static float fInvalid = -1.0f;
	return (fInvalid);
}

bool TVector3::operator == (const TVector3&_kRight) const
{
	return (FW_MEMCOMPARE(this, &_kRight, sizeof(TVector3)) == 0);
}

bool TVector3::operator != (const TVector3&_kRight) const
{
	return (FW_MEMCOMPARE(this, &_kRight, sizeof(TVector3)) != 0);
}