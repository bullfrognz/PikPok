
//
//  File Name   :   WorldMatrix.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "WorldMatrix.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/Vector3.h"
#include "Library/Matrix.h"
#include "Library/MathUtilities.h"
#include "Defines/Macros.h"

// Library Includes
#include <DirectX/D3DX10.h>

// Static Initialisers

// Implementation

TWorldMatrix::TWorldMatrix()
{
	TMatrix::Identity();
}

TWorldMatrix::TWorldMatrix(TMatrix& _xrMatrix)
{
	FW_MEMCOPY(this, &_xrMatrix, sizeof(TMatrix));
}

TWorldMatrix::~TWorldMatrix()
{
	// Empty
}

void TWorldMatrix::Scale(float _fScale)
{
	Scale(_fScale, _fScale, _fScale);
}

void TWorldMatrix::Scale(const TVector3& _kvrScale)
{
	Scale(_kvrScale.x, _kvrScale.y, _kvrScale.z);
}

void TWorldMatrix::Scale(float _fX, float _fY, float _fZ)
{
	TVector3 tPosition = Position();
	Position(0.0f, 0.0f, 0.0f);

	D3DXMatrixScaling(reinterpret_cast<D3DXMATRIX*>(this), _fX, _fY, _fZ);

	Position(tPosition);
}

void TWorldMatrix::RotateRadians(float _fPitch, float _fYaw, float _fRoll)
{
	TMatrix xTemp;
	D3DXMatrixRotationYawPitchRoll(reinterpret_cast<D3DXMATRIX*>(&xTemp), _fYaw, _fPitch, _fRoll);

	*this *= xTemp;
}

void TWorldMatrix::LookAt(const TVector3& _kvrTargetPosition)
{
	DEBUG_ERROR("Method not implemented");
}

void TWorldMatrix::PositionX(float _fPositionX)
{
	values[3][0] = _fPositionX;
}

void TWorldMatrix::PositionY(float _fPositionY)
{
	values[3][1] = _fPositionY;
}

void TWorldMatrix::PositionZ(float _fPositionZ)
{
	values[3][2] = _fPositionZ;
}

void TWorldMatrix::Position(float _fPositionX, float _fPositionY, float _fPositionZ)
{
	values[3][0] = _fPositionX;
	values[3][1] = _fPositionY;
	values[3][2] = _fPositionZ;
}

void TWorldMatrix::Position(const TVector3& _kv3rPosition)
{
	values[3][0] = _kv3rPosition.x;
	values[3][1] = _kv3rPosition.y;
	values[3][2] = _kv3rPosition.z;
}

void TWorldMatrix::TranslateX(float _fPositionX)
{
	values[3][0] += _fPositionX;
}

void TWorldMatrix::TranslateY(float _fPositionY)
{
	values[3][1] += _fPositionY;
}

void TWorldMatrix::TranslateZ(float _fPositionZ)
{
	values[3][2] += _fPositionZ;
}

void TWorldMatrix::Translate(float _fPositionX, float _fPositionY, float _fPositionZ)
{
	values[3][0] += _fPositionX;
	values[3][1] += _fPositionY;
	values[3][2] += _fPositionZ;
}

void TWorldMatrix::Translate(const TVector3& _kv3rPosition)
{
	values[3][0] += _kv3rPosition.x;
	values[3][1] += _kv3rPosition.y;
	values[3][2] += _kv3rPosition.z;
}

void TWorldMatrix::TranslateTowards(const TVector3& _ktrTarget, float _fDeltaSpeed)
{
	TVector3 Displacement = _ktrTarget.DisplacementTo(Position());
	Displacement.Normalise();

	TVector3 TranslateDelta = Displacement * _fDeltaSpeed;

	PositionX(MathUtilities::AddClamp<float>(PositionX(), TranslateDelta.x, _ktrTarget.x));
	PositionY(MathUtilities::AddClamp<float>(PositionY(), TranslateDelta.y, _ktrTarget.y));
	PositionZ(MathUtilities::AddClamp<float>(PositionZ(), TranslateDelta.z, _ktrTarget.z));
}

TVector3 TWorldMatrix::Position() const
{
	return (TVector3(values[3][0], values[3][1], values[3][2]));
}

float TWorldMatrix::PositionX() const
{
	return (values[3][0]);
}

float TWorldMatrix::PositionY() const
{
	return (values[3][1]);
}

float TWorldMatrix::PositionZ() const
{
	return (values[3][2]);
}

TVector3 TWorldMatrix::OrientationX() const
{
	return (TVector3(values[0][0], values[0][1], values[0][2]));
}

TVector3 TWorldMatrix::OrientationY() const
{
	return (TVector3(values[1][0], values[1][1], values[1][2]));
}

TVector3 TWorldMatrix::OrientationZ() const
{
	return (TVector3(values[2][0], values[2][1], values[2][2]));
}