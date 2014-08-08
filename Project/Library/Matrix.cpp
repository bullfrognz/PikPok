
//
//  File Name   :   Matrix.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Matrix.h"

// Local Includes
#include "Framework/Debug.h"
#include "Defines/Macros.h"
#include "Library/Quaternion.h"
#include "Library/Vector4.h"
#include "Library/Vector3.h"
#include "Defines/Types.h"

// Library Includes
#include <DirectX/D3DX10.h>

// Static Initialisers

// Implementation

TMatrix::TMatrix()
{
	Identity();
}

TMatrix::TMatrix(const TQuaternion& _kxrQuaternion)
{
	D3DXMatrixRotationQuaternion(reinterpret_cast<D3DXMATRIX*>(this), reinterpret_cast<const D3DXQUATERNION*>(&_kxrQuaternion));
}

TMatrix::~TMatrix()
{
	// Empty
}

TMatrix& TMatrix::Identity()
{
	D3DXMatrixIdentity(reinterpret_cast<D3DXMATRIX*>(this));

	return (*this);
}

TMatrix& TMatrix::Inverse(TMatrix& _rTarget) const
{
	float fDeterminant = Determinant();

	D3DXMatrixInverse(reinterpret_cast<D3DXMATRIX*>(&_rTarget), &fDeterminant, reinterpret_cast<const D3DXMATRIX*>(this));

	return (_rTarget);
}

TMatrix& TMatrix::Invert()
{
	float fDeterminant = Determinant();

	D3DXMatrixInverse(reinterpret_cast<D3DXMATRIX*>(this), &fDeterminant, reinterpret_cast<const D3DXMATRIX*>(this));

	return (*this);
}

float TMatrix::Determinant() const
{
	float fDeterminant = D3DXMatrixDeterminant(reinterpret_cast<const D3DXMATRIX*>(this));

	return (fDeterminant);
}

TVector4& TMatrix::TransformPoint(const TVector3& _kvrPoint, TVector4& _vrResult) const
{
	D3DXVec3Transform(reinterpret_cast<D3DXVECTOR4*>(&_vrResult),
					  reinterpret_cast<const D3DXVECTOR3*>(&_kvrPoint),
					  reinterpret_cast<const D3DXMATRIX*>(this));

	return (_vrResult);
}

bool TMatrix::operator == (const TMatrix& _krMatrix) const
{
	int iMemResult = memcmp(this, &_krMatrix, sizeof(TMatrix));

	return (iMemResult == 0);
}

bool TMatrix::operator != (const TMatrix& _krMatrix) const
{
	int iMemResult = memcmp(this, &_krMatrix, sizeof(TMatrix));

	return (iMemResult != 0);;
}

TMatrix& TMatrix::operator += (const TMatrix& _krMatrix)
{
	reinterpret_cast<D3DXMATRIX&>(*this) += reinterpret_cast<const D3DXMATRIX&>(_krMatrix);

	return (*this);
}

TMatrix& TMatrix::operator -= (const TMatrix& _krMatrix)
{
	reinterpret_cast<D3DXMATRIX&>(*this) -= reinterpret_cast<const D3DXMATRIX&>(_krMatrix);

	return (*this);
}

TMatrix& TMatrix::operator *= (const TMatrix& _krMatrix)
{
	D3DXMatrixMultiply(reinterpret_cast<D3DXMATRIX*>(this), reinterpret_cast<const D3DXMATRIX*>(this), reinterpret_cast<const D3DXMATRIX*>(&_krMatrix));

	return (*this);
}

TMatrix& TMatrix::operator  = (const TMatrix& _kxrMatrix)
{
	FW_MEMCOPY(this, &_kxrMatrix, sizeof(float)* 4 * 4);

	return (*this);
}

TMatrix TMatrix::operator + (const TMatrix& _krMatrix) const
{
	TMatrix mtxTempMatrix(*this);
	mtxTempMatrix += _krMatrix;

	return (mtxTempMatrix);
}

TMatrix TMatrix::operator - (const TMatrix& _krMatrix) const
{
	TMatrix mtxTempMatrix(*this);
	mtxTempMatrix -= _krMatrix;

	return (mtxTempMatrix);
}

TMatrix TMatrix::operator * (const TMatrix& _krMatrix) const
{
	TMatrix mtxTempMatrix(*this);
	mtxTempMatrix *= _krMatrix;

	return (mtxTempMatrix);
}