//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  File Name :   FPCamera.cpp
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Library Includes


// Defines Includes
#include "Defines/Macros.h"


// Local Includes
#include "Library/WorldMatrix.h"
#include "Library/ViewMatrix.h"
#include "Framework/Logger.h"
#include "Framework/Keyboard.h"
#include "Framework/Pointer.h"


// This Include
#include "FPCamera.h"


// Static Variables


// Implementation


/********************************
            Public
*********************************/


/*---------------------------------------------------------------------------------------------------------------------------
*
* CFPCamera constructor
*
* @author Bryce Booth
*
*---------------------------------------------------------------------------------------------------------------------------*/

CFPCamera::CFPCamera()
: m_fMovementVelocity(100.0f)
, m_fPointerTranslationX(0)
, m_fPointerTranslationY(0)
, m_fSensitivity(0.002f)
, m_bMoveForward(false)
, m_bMoveBack(false)
, m_bMoveLeft(false)
, m_bMoveRight(false)
, m_bMoveUp(false)
, m_bMoveDown(false)
, m_bRotationEnabled(true)
, m_bMovementEnabled(true)
{
	// Empty
}


/*---------------------------------------------------------------------------------------------------------------------------
*
* CFPCamera deconstruction
*
* @author Bryce Booth
*
*---------------------------------------------------------------------------------------------------------------------------*/

CFPCamera::~CFPCamera()
{
	POINTER.UnsubscribeAll(this);
	KEYBOARD.UnsubscribeAll(this);


	POINTER.SetVisible(true);
}


bool
CFPCamera::Initialise(EDisplayType _eDisplayType, uint _uiWidth, uint _uiHeight)
{
	VALIDATE(InitialiseRecipient());


	CCamera::Initialise(_eDisplayType, _uiWidth, _uiHeight);


	NotifyDeactivated();


	return (true);
}


bool
CFPCamera::Process(float _fDeltatick)
{
	ProcessRotation(_fDeltatick);
	ProcessMovement(_fDeltatick);


	return (true);
}


void
CFPCamera::Activate()
{
	SetRotationEnabled(true);
	SetMovementEnabled(true);


	POINTER.SetVisible(false);


	CCamera::Activate();
}


void
CFPCamera::NotifyDeactivated()
{
	SetRotationEnabled(false);
	SetMovementEnabled(false);


	POINTER.SetVisible(true);


	CCamera::NotifyDeactivated();
}


void
CFPCamera::Notify(IPointer& _rSubject, short _sSubject, c_void* _kpData)
{
	/*
	switch (_sSubject)
	{
	case IPointer::SUBJECT_MOVE:
		m_fPointerTranslationX = static_cast<float>(_rSubject.GetTranslationX());
		m_fPointerTranslationY = static_cast<float>(_rSubject.GetTranslationY());
		break;
	}
	*/
}


void
CFPCamera::Notify(IKeyboard& _rSubject, short _sSubject, c_void* _kpData)
{
	bool bDown = false;

	switch (_sSubject)
	{
	case IKeyboard::KEY_W:			m_bMoveForward	= bDown; break;
	case IKeyboard::KEY_S:			m_bMoveBack		= bDown; break;
	case IKeyboard::KEY_A:			m_bMoveLeft		= bDown; break;
	case IKeyboard::KEY_D:			m_bMoveRight	= bDown; break;
	case IKeyboard::KEY_SPACE:		m_bMoveUp		= bDown; break;
	case IKeyboard::KEY_CONTROL_L:	m_bMoveDown		= bDown; break;
	}
}


/********************************
            Protected
*********************************/


void
CFPCamera::ProcessRotation(float _fDeltatick)
{
	if (m_bRotationEnabled)
	{
		//Check position has changed
		if (m_fPointerTranslationX || m_fPointerTranslationY)
		{
			CEntity::AddRotationY(m_fPointerTranslationX * m_fSensitivity);
			CEntity::AddRotationX(m_fPointerTranslationY * m_fSensitivity);
		}


		m_fPointerTranslationX = 0;
		m_fPointerTranslationY = 0;


		LimitRotation();


		POINTER.Center();
	}
}


void
CFPCamera::ProcessMovement(float _fDeltatick)
{
	if (m_bMovementEnabled)
	{
		const CWorldMatrix& kxrWorld = CEntity::GetWorld();


		CVector3 vOrientationX = kxrWorld.GetOrientationX();
		CVector3 vOrientationZ = kxrWorld.GetOrientationZ();


		CVector3 vTranslationX;
		CVector3 vTranslationY;
		CVector3 vTranslationZ;


		// Forward - Back
		if (m_bMoveForward && !m_bMoveBack)
		{
			vTranslationZ = vOrientationZ * m_fMovementVelocity;
		}
		else if (!m_bMoveForward && m_bMoveBack)
		{
			vTranslationZ = vOrientationZ * -m_fMovementVelocity;
		}


		// Left - Right
		if (m_bMoveLeft && !m_bMoveRight)
		{
			vTranslationX = vOrientationX * -m_fMovementVelocity;
		}
		else if (!m_bMoveLeft && m_bMoveRight)
		{
			vTranslationX = vOrientationX * m_fMovementVelocity;
		}


		// Up - Down
		if (m_bMoveUp && !m_bMoveDown)
		{
			vTranslationY.SetY(m_fMovementVelocity);
		}
		else if (!m_bMoveUp && m_bMoveDown)
		{
			vTranslationY.SetY(-m_fMovementVelocity);
		}


		if (!vTranslationX.IsZero() ||
			!vTranslationY.IsZero() ||
			!vTranslationZ.IsZero())
		{
			CEntity::Translate( vTranslationX * _fDeltatick );
			CEntity::Translate( vTranslationY * _fDeltatick );
			CEntity::Translate( vTranslationZ * _fDeltatick );
		}
	}
}


void
CFPCamera::LimitRotation()
{
	if (CEntity::GetRotation().GetX() > 1.221f)
	{
		CEntity::SetRotationX(1.221f);
	}
	else if (CEntity::GetRotation().GetX() < -1.221)
	{
		CEntity::SetRotationX(-1.221f);
	}
}


/********************************
            Private
*********************************/


bool
CFPCamera::InitialiseRecipient()
{
	POINTER.Subscribe(this, IPointer::SUBJECT_MOVE);

	KEYBOARD.Subscribe(this, IKeyboard::KEY_W);
	KEYBOARD.Subscribe(this, IKeyboard::KEY_S);
	KEYBOARD.Subscribe(this, IKeyboard::KEY_A);
	KEYBOARD.Subscribe(this, IKeyboard::KEY_D);
	KEYBOARD.Subscribe(this, IKeyboard::KEY_SPACE);
	KEYBOARD.Subscribe(this, IKeyboard::KEY_CONTROL_L);

	return (true);
}