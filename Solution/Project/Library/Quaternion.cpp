
//
//  File Name   :   Quaternion.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Quaternion.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes
#include <DirectX/D3Dx10.h>

// Static Initialisers

// Implementation

#define ConvertQuaternion(pVariable)		reinterpret_cast<D3DXQUATERNION*>(pVariable)
#define ConvertConstQuaternion(pVariable)	reinterpret_cast<const D3DXQUATERNION*>(pVariable)

TQuaternion::TQuaternion()
: x(0.0f)
, y(0.0f)
, z(0.0f)
, w(0.0f)
{
	// Empty
}

TQuaternion::TQuaternion(const TQuaternion& _kvrRight)
: x(_kvrRight.x)
, y(_kvrRight.y)
, z(_kvrRight.z)
, w(_kvrRight.w)
{
	// Empty
}

TQuaternion::~TQuaternion()
{
	// Empty
}

TQuaternion& TQuaternion::RotateX(float _fX)
{
	D3DXQuaternionRotationAxis(ConvertQuaternion(this), &D3DXVECTOR3(1.0f, 0.0f, 0.0f), _fX);

	return (*this);
}

TQuaternion& TQuaternion::RotateY(float _fY)
{
	D3DXQuaternionRotationAxis(ConvertQuaternion(this), &D3DXVECTOR3(0.0f, 1.0f, 0.0f), _fY);

	return (*this);
}

TQuaternion& TQuaternion::RotateZ(float _fZ)
{
	D3DXQuaternionRotationAxis(ConvertQuaternion(this), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), _fZ);

	return (*this);
}

TQuaternion& TQuaternion::Rotate(float _fX, float _fY, float _fZ)
{
	D3DXQuaternionRotationYawPitchRoll(ConvertQuaternion(this), _fY, _fX, _fZ);

	return (*this);
}

TQuaternion& TQuaternion::SetIdentity()
{
	D3DXQuaternionIdentity(ConvertQuaternion(this));

	return (*this);
}

TQuaternion& TQuaternion::SetInvert()
{
	D3DXQuaternionInverse(ConvertQuaternion(this), ConvertQuaternion(this));

	return (*this);
}

TQuaternion& TQuaternion::SetConjugate()
{
	D3DXQuaternionConjugate(ConvertQuaternion(this), ConvertQuaternion(this));

	return (*this);
}

TQuaternion& TQuaternion::GetInserse(TQuaternion& _vrRight)
{
	D3DXQuaternionInverse(ConvertQuaternion(&_vrRight), ConvertQuaternion(this));

	return (_vrRight);
}

TQuaternion& TQuaternion::GetConjugate(TQuaternion& _vrRight)
{
	D3DXQuaternionConjugate(ConvertQuaternion(&_vrRight), ConvertQuaternion(this));

	return (_vrRight);
}

TQuaternion TQuaternion::GetInserse()
{
	TQuaternion vTemp;

	D3DXQuaternionInverse(ConvertQuaternion(&vTemp), ConvertQuaternion(this));

	return (vTemp);
}

TQuaternion TQuaternion::GetConjugate()
{
	TQuaternion vTemp;

	D3DXQuaternionConjugate(ConvertQuaternion(&vTemp), ConvertQuaternion(this));

	return (vTemp);
}

TQuaternion& TQuaternion::operator += (const TQuaternion& _kvrRight)
{
	*ConvertQuaternion(this) += *ConvertConstQuaternion(&_kvrRight);

	return (*this);
}

TQuaternion& TQuaternion::operator -= (const TQuaternion& _kvrRight)
{
	*ConvertQuaternion(this) -= *ConvertConstQuaternion(&_kvrRight);

	return (*this);
}

TQuaternion& TQuaternion::operator *= (const TQuaternion& _kvrRight)
{
	*ConvertQuaternion(this) *= *ConvertConstQuaternion(&_kvrRight);

	return (*this);
}

TQuaternion& TQuaternion::operator *= (float _fRight)
{
	*ConvertQuaternion(this) *= _fRight;

	return (*this);
}

TQuaternion& TQuaternion::operator /= (float _fRight)
{
	*ConvertQuaternion(this) /= _fRight;

	return (*this);
}

TQuaternion TQuaternion::operator + (const TQuaternion& _kvrRight)
{
	return (reinterpret_cast<TQuaternion&>(reinterpret_cast<D3DXQUATERNION&>(*this) + reinterpret_cast<const D3DXQUATERNION&>(_kvrRight)));
}

TQuaternion TQuaternion::operator - (const TQuaternion& _kvrRight)
{
	return (reinterpret_cast<TQuaternion&>(*ConvertQuaternion(this) + *ConvertConstQuaternion(&_kvrRight)));
}

TQuaternion TQuaternion::operator * (const TQuaternion& _kvrRight)
{
	return (reinterpret_cast<TQuaternion&>(*ConvertQuaternion(this) * *ConvertConstQuaternion(&_kvrRight)));
}

TQuaternion TQuaternion::operator * (float _fRight)
{
	return (reinterpret_cast<TQuaternion&>(*ConvertQuaternion(this) * _fRight));
}

TQuaternion TQuaternion::operator / (float _fRight)
{
	return (reinterpret_cast<TQuaternion&>(*ConvertQuaternion(this) / _fRight));
}

