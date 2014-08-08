
//
//  File Name   :   Camera.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Camera.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Application.h"
#include "Framework/Renderer.h"
#include "Library/AABox.h"

// Library Includes
#include <DirectX/D3DX10math.h>

// Static Initialisers

// Implementation

CCamera::CCamera()
: m_eDisplay(INVALID_DISPLAYTYOE)
, m_fWidth(0)
, m_fHeight(0)
, m_bViewDirty(true)
{
	// Empty
}

CCamera::~CCamera()
{
	if (&RENDERER.Camera() == this)
	{
		RENDERER.Camera(0);
	}
}

bool CCamera::Initialise(EDisplayType _eDisplayType, float _fWidth, float _fHeight)
{
	SetDisplayType(_eDisplayType);
	SetDimentions(_fWidth, _fHeight);

	return (true);
}

bool CCamera::IsInFrustum(const TAABox& _rAABoundingBox) const
{
	bool bInFrustum = true;
	TVector3 vPlaneVertex;

	//Loop through all planes
	for (int iPlane = 0; iPlane < TProjectionMatrix::MAX_FRUSTUMPLANE; ++ iPlane)
	{
		// x-axis
		if(m_FrustumPlane[iPlane].x < 0.0f)	// Which AABB vertex is furthest down (plane normals direction) the x axis
			vPlaneVertex.x =  _rAABoundingBox.m_vCenter.x + _rAABoundingBox.m_fSideOffset[TAABox::SIDE_LEFT]; // min x plus tree positions x
		else
			vPlaneVertex.x =  _rAABoundingBox.m_vCenter.x + _rAABoundingBox.m_fSideOffset[TAABox::SIDE_RIGHT]; // max x plus tree positions x

		// y-axis
		if(m_FrustumPlane[iPlane].y < 0.0f)	// Which AABB vertex is furthest down (plane normals direction) the y axis
			vPlaneVertex.y =  _rAABoundingBox.m_vCenter.y + _rAABoundingBox.m_fSideOffset[TAABox::SIDE_BOTTOM]; // min y plus tree positions y
		else
			vPlaneVertex.y =  _rAABoundingBox.m_vCenter.y + _rAABoundingBox.m_fSideOffset[TAABox::SIDE_TOP]; // max y plus tree positions y
			
		// z-axis
		if(m_FrustumPlane[iPlane].z < 0.0f)	// Which AABB vertex is furthest down (plane normals direction) the z axis
			vPlaneVertex.z =  _rAABoundingBox.m_vCenter.z + _rAABoundingBox.m_fSideOffset[TAABox::SIDE_BACK]; // min z plus tree positions z
		else
			vPlaneVertex.z =  _rAABoundingBox.m_vCenter.z + _rAABoundingBox.m_fSideOffset[TAABox::SIDE_FRONT]; // max z plus tree positions z

		// Now we get the signed distance from the AABB vertex that's furthest down the frustum planes normal,
		// and if the signed distance is negative, then the entire bounding box is behind the frustum plane, which means
		// that it should be culled
		if(D3DXPlaneDotCoord(reinterpret_cast<const D3DXPLANE*>(&m_FrustumPlane[iPlane]), reinterpret_cast<const D3DXVECTOR3*>(&vPlaneVertex)) < 0.0f) // + m_FrustumPlane[iPlane].d
		{
			bInFrustum = false;

			break;
		}
	}

	return(bInFrustum);
}

void CCamera::SetDisplayType(EDisplayType _eDisplay)
{
	m_eDisplay = _eDisplay;

	RegenerateProjection();
}

void CCamera::SetDimentions(float _fWidth, float _fHeight)
{
	m_fWidth	= _fWidth;
	m_fHeight	= _fHeight;

	RegenerateProjection();
}

float CCamera::Width() const
{
	return (m_fWidth);
}

float CCamera::Height() const
{
	return (m_fHeight);
}

TViewMatrix& CCamera::View()
{
	RegenerateView();

	return (m_xView);
}

TProjectionMatrix& CCamera::Projection()
{
	return (m_xProjection);
}

void CCamera::RegenerateView()
{
	CEntity::World().Inverse(m_xView);
	RegenerateFrustumPlanes();

	m_bViewDirty = false;
}

void CCamera::RegenerateFrustumPlanes()
{
	for (uint i = 0; i < TProjectionMatrix::MAX_FRUSTUMPLANE; ++ i)
	{
		m_xProjection.GenerateFrustumPlane(m_xView, static_cast<TProjectionMatrix::EFrustumPlane>(i), m_FrustumPlane[i]);
	}
}

void CCamera::RegenerateProjection()
{
	switch (m_eDisplay)
	{
	case CCamera::DISPLAYTYPE_ORTHOGAPHIC:
	{
		 m_xProjection.SetOrthographicFovLH(m_fWidth, m_fHeight, 0.1f, 1000.0f);
		break;
	}

	case CCamera::DISPLAYTYPE_PERSPECTIVE:
	{
		float fAspectRatio = m_fWidth / m_fHeight;
		m_xProjection.SetPerspectiveFovLH(1.047f, fAspectRatio, 0.1f, 1000.0f);	
		break;
	}

	default:
		DEBUG_ERROR("Unknown display type.");
		break;
	}

	RegenerateFrustumPlanes();
}