
//
//  File Name   :   ProjectionMatrix.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "ProjectionMatrix.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/ViewMatrix.h"
#include "Library/Vector4.h"

// Library Includes
#include <DirectX/D3DX10math.h>

// Static Initialisers

// Implementation

TProjectionMatrix::TProjectionMatrix()
{
	// Empty
}

TProjectionMatrix::~TProjectionMatrix()
{
	// Empty
}

void TProjectionMatrix::SetPerspectiveFovLH(float _fFoV, float _fAspectRatio, float _fMin, float _fMax)
{
	D3DXMatrixPerspectiveFovLH(reinterpret_cast<D3DXMATRIX*>(this), _fFoV, _fAspectRatio, _fMin, _fMax);
}

void TProjectionMatrix::SetOrthographicFovLH(float _fWidth, float _fHeight, float _fMin, float _fMax)
{
	D3DXMatrixOrthoLH(reinterpret_cast<D3DXMATRIX*>(this), _fWidth, _fHeight, _fMin, _fMax);
}

void TProjectionMatrix::GenerateFrustumPlane(const TViewMatrix& _kxrView, EFrustumPlane _eFrustumPlane, TVector4& _rPlane)
{
	D3DXMATRIX mtxViewProjection;
	D3DXMatrixMultiply(&mtxViewProjection, reinterpret_cast<const D3DXMATRIX*>(&_kxrView), reinterpret_cast<const D3DXMATRIX*>(this));

	if (_eFrustumPlane == FRUSTUMPLANE_LEFT)
	{
		// Left plane
		_rPlane.x = mtxViewProjection._14 + mtxViewProjection._11;
		_rPlane.y = mtxViewProjection._24 + mtxViewProjection._21;
		_rPlane.z = mtxViewProjection._34 + mtxViewProjection._31;
		_rPlane.w = mtxViewProjection._44 + mtxViewProjection._41;
	}
	else if (_eFrustumPlane == FRUSTUMPLANE_RIGHT)
	{
		// Right plane
		_rPlane.x = mtxViewProjection._14 - mtxViewProjection._11;
		_rPlane.y = mtxViewProjection._24 - mtxViewProjection._21;
		_rPlane.z = mtxViewProjection._34 - mtxViewProjection._31;
		_rPlane.w = mtxViewProjection._44 - mtxViewProjection._41;
	}
	else if (_eFrustumPlane == FRUSTUMPLANE_TOP)
	{
		// Top plane
		_rPlane.x = mtxViewProjection._14 - mtxViewProjection._12;
		_rPlane.y = mtxViewProjection._24 - mtxViewProjection._22;
		_rPlane.z = mtxViewProjection._34 - mtxViewProjection._32;
		_rPlane.w = mtxViewProjection._44 - mtxViewProjection._42;
	}
	else if (_eFrustumPlane == FRUSTUMPLANE_BOTTOM)
	{
		// Bottom plane
		_rPlane.x = mtxViewProjection._14 + mtxViewProjection._12;
		_rPlane.y = mtxViewProjection._24 + mtxViewProjection._22;
		_rPlane.z = mtxViewProjection._34 + mtxViewProjection._32;
		_rPlane.w = mtxViewProjection._44 + mtxViewProjection._42;
	}
	else if (_eFrustumPlane == FRUSTUMPLANE_NEAR)
	{
		// Near plane
		_rPlane.x = mtxViewProjection._13;
		_rPlane.y = mtxViewProjection._23;
		_rPlane.z = mtxViewProjection._33;
		_rPlane.w = mtxViewProjection._43;
	}
	else if (_eFrustumPlane == FRUSTUMPLANE_FAR)
	{
		// Far plane
		_rPlane.x = mtxViewProjection._14 - mtxViewProjection._13;
		_rPlane.y = mtxViewProjection._24 - mtxViewProjection._23;
		_rPlane.z = mtxViewProjection._34 - mtxViewProjection._33;
		_rPlane.w = mtxViewProjection._44 - mtxViewProjection._43;
	}
	else
	{
		DEBUG_ERROR("Unknown frustum plane");
	}

	D3DXPlaneNormalize(reinterpret_cast<D3DXPLANE*>(&_rPlane), reinterpret_cast<const D3DXPLANE*>(&_rPlane));
}