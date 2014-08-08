
//
//  File Name   :   Light.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Light.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Application.h"
#include "Framework/Renderer.h"

// Library Includes

// Static Initialisers

// Implementation

TLight::TLight()
: m_vDiffuse(0.85f, 0.85f, 0.85f, 1.0f)
, m_vAmbient(1.0f, 1.0f, 1.0f, 1.0f)
, m_vSpecular(0.0f, 0.0f, 0.0f, 1.0f)
, m_fPadding1(0)
, m_fPadding2(0)
, m_fAttenuation0(0)
, m_fAttenuation1(0)
, m_fAttenuation2(0)
, m_fRange(0)
, m_fTheta(0)
, m_fPhi(0)
, m_fFalloff(0)
, m_eType(INVALID_TYPE)
{
	// Empty
}


TLight::TLight(EType _eType)
: m_vDiffuse(0.85f, 0.85f, 0.85f, 1.0f)
, m_vAmbient(1.0f, 1.0f, 1.0f, 1.0f)
, m_vSpecular(0.0f, 0.0f, 0.0f, 1.0f)
, m_fPadding1(0)
, m_fPadding2(0)
, m_fAttenuation0(0)
, m_fAttenuation1(0)
, m_fAttenuation2(0)
, m_fRange(0)
, m_fTheta(0)
, m_fPhi(0)
, m_fFalloff(0)
, m_eType(_eType)
{
	// Empty
}


TLight::~TLight()
{
	TurnOff();
}


void
TLight::TurnOn()
{
	RENDERER.AddLight(this);
}


void
TLight::TurnOff()
{
	RENDERER.RemoveLight(this);
}