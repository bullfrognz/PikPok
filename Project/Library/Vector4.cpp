
//
//  File Name   :   Vector4.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Vector4.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/Vector2.h"
#include "Library/Vector3.h"
#include "Library/Matrix.h"

// Library Includes
#include <DirectX/D3DX10math.h>

// Static Initialisers

// Implementation

TVector4::TVector4()
: x(0)
, y(0)
, z(0)
, w(0)
{
	// Empty
}

TVector4::TVector4(const TVector2& _krVector)
: x(_krVector.x)
, y(_krVector.y)
, z(0.0f)
, w(1.0f)
{

}

TVector4::TVector4(const TVector3& _krVector)
: x(_krVector.x)
, y(_krVector.y)
, z(_krVector.z)
, w(1.0f)
{

}

TVector4::TVector4(const TVector4& _krVector)
: x(_krVector.x)
, y(_krVector.y)
, z(_krVector.z)
, w(_krVector.w)
{
	// Empty
}

TVector4::TVector4(float _fX, float _fY, float _fZ, float _fW)
: x(_fX)
, y(_fY)
, z(_fZ)
, w(_fW)
{
}

TVector4::~TVector4()
{
	// Empty
}

void TVector4::Set(float _fX, float _fY, float _fZ, float _fW)
{
	x = _fX;
	y = _fY;
	z = _fZ;
	w = _fW;
}

TVector4& TVector4::operator *= (const TMatrix& _kxrMatrix)
{
	D3DXVec4Transform(reinterpret_cast<D3DXVECTOR4*>(this), reinterpret_cast<const D3DXVECTOR4*>(this), reinterpret_cast<const D3DXMATRIX*>(&_kxrMatrix));

	return (*this);
}

TVector4 TVector4::operator * (const TMatrix& _kxrMatrix)
{
	TVector4 vConverted;
	D3DXVec4Transform(reinterpret_cast<D3DXVECTOR4*>(&vConverted), reinterpret_cast<const D3DXVECTOR4*>(this), reinterpret_cast<const D3DXMATRIX*>(&_kxrMatrix));

	return (vConverted);
}