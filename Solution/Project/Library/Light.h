
//
//  File Name   :   Light.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Light_H__
#define __Light_H__

// Dependent Includes

// Local Includes
#include "Library/Vector3.h"
#include "Library/Colour.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes

struct TLight
{

// Member Types

	enum EType
	{
		INVALID_TYPE = -1,

		TYPE_DIRECTIONAL,
		TYPE_POINT,
		TYPE_SPOT,

		MAX_TYPE
	};

// Member Functions

	 TLight();
	 TLight(EType _eType);
	~TLight();

	void TurnOn();
	void TurnOff();

// Member Variables

	TVector3	m_vPosition;
	float		m_fPadding1;

	TVector3	m_vDirection;
	float		m_fPadding2;

	TColour		m_vDiffuse;
	TColour		m_vAmbient;
	TColour		m_vSpecular;

	float		m_fAttenuation0;
	float		m_fAttenuation1;
	float		m_fAttenuation2;

	float		m_fRange;
	float		m_fTheta;
	float		m_fPhi;
	float		m_fFalloff;

	EType		m_eType;

};

#endif //__Light_H__