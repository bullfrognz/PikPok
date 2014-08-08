
//
//  File Name   :   ViewMatrix.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "ViewMatrix.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/Vector3.h"

// Library Includes
#include <DirectX/D3DX10.h>

// Static Initialisers

// Implementation

TViewMatrix::TViewMatrix()
{
	TMatrix::Identity();
}


TViewMatrix::~TViewMatrix()
{
	// Empty
}

void TViewMatrix::LookAtLH(const TVector3& _krPosition, const TVector3& _krTarget)
{
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(reinterpret_cast<D3DXMATRIX*>(this),
					   reinterpret_cast<const D3DXVECTOR3*>(&_krPosition),
					   reinterpret_cast<const D3DXVECTOR3*>(&_krTarget),
					   &Up);
}

void TViewMatrix::GetLookAt(TVector3& _rLookAt) const
{
	_rLookAt.x = values[0][2];
	_rLookAt.y = values[0][2];
	_rLookAt.z = values[0][2];
}

void TViewMatrix::GetRight(TVector3& _rRight) const
{
	_rRight.x = values[0][0];
	_rRight.y = values[1][0];
	_rRight.z = values[2][0];
}

void TViewMatrix::GetUp(TVector3& _rUp) const
{
	_rUp.x = values[0][1];
	_rUp.y = values[1][1];
	_rUp.z = values[2][1];
}

TVector3 TViewMatrix::GetLookAt() const
{
	return (TVector3(values[0][2], values[1][2], values[2][2]));
}

TVector3 TViewMatrix::GetRight() const
{
	return (TVector3(values[0][0], values[1][0], values[2][0]));
}

TVector3 TViewMatrix::GetUp() const
{
	return (TVector3(values[0][1], values[1][1], values[2][1]));
}