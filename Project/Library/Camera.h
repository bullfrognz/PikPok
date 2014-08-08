
//
//  File Name   :   Camera.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Camera_H__
#define __Camera_H__

// Dependent Includes
#include "Library/Entity.h"

// Local Includes
#include "Library/ViewMatrix.h"
#include "Library/ProjectionMatrix.h"
#include "Library/Vector4.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TAABox;

class CCamera : public CEntity
{

	// Member Types
public:

	enum EDisplayType
	{
		INVALID_DISPLAYTYOE,
		DISPLAYTYPE_ORTHOGAPHIC,
		DISPLAYTYPE_PERSPECTIVE
	};

	// Member Functions
public:

			 CCamera();
	virtual ~CCamera();

	virtual bool Initialise(EDisplayType _eDisplayType, float _fWidth, float _fHeight);

	void SetDisplayType(EDisplayType _eDisplay);
	void SetDimentions(float _fWidth, float _fHeight);

	bool IsInFrustum(const TAABox& _rAABoundingBox) const;

	float				Width()			const;
	float				Height()		const;
	TViewMatrix&		View();
	TProjectionMatrix&	Projection();

protected:

	void RegenerateView();
	void RegenerateFrustumPlanes();
	void RegenerateProjection();

private:

	CCamera(const CCamera& _krCamera);
	CCamera& operator = (const CCamera& _krCamera);

	// Member Variables
protected:

private:

	TViewMatrix			m_xView;
	TProjectionMatrix	m_xProjection;
	TVector4			m_FrustumPlane[TProjectionMatrix::MAX_FRUSTUMPLANE];

	EDisplayType		m_eDisplay;

	float				m_fWidth;
	float				m_fHeight;

	bool				m_bViewDirty;

};

#endif //__Camera_H__