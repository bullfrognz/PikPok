//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name   :   FPCamera.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef __FPCamera_H__
#define __FPCamera_H__


// Library Includes


// Local Includes
#include "Library/Camera.h"
#include "Library/Observer.h"
#include "Framework/Window.h"


// Types


// Constants


// Prototypes
class IPointer;
class IKeyboard;


class CFPCamera : public CCamera, public IObserver<IPointer>, public IObserver<IKeyboard>
{

	// Member Functions
public:


			 CFPCamera();
	virtual ~CFPCamera();


	virtual bool Initialise(EDisplayType _eDisplayType, uint _uiWidth, uint _uiHeight);
	virtual bool Process(float _fDeltatick);


	virtual void Activate();
	virtual void NotifyDeactivated();


	// Inline Functions
	inline void SetRotationEnabled(bool _bEnabled);
	inline void SetMovementEnabled(bool _bEnabled);
	inline void SetSensitivity(float _fSensitivity);
	inline void SetMovementVelocity(float _fVelocity);


protected:


	void ProcessRotation(float _fDeltatick);
	void ProcessMovement(float _fDeltatick);


	void LimitRotation();


private:


	bool InitialiseRecipient();


	virtual void Notify(IPointer& _rSubject, short _sSubject, c_void* _kpData);
	virtual void Notify(IKeyboard& _rSubject, short _sSubject, c_void* _kpData);


	CFPCamera(const CFPCamera& _krFPCamera);
	CFPCamera& operator = (const CFPCamera& _krFPCamera);


	// Member Variables
protected:


private:


	CVector3 m_Velocity;


	float m_fMovementVelocity;
	float m_fPointerTranslationX;
	float m_fPointerTranslationY;
	float m_fSensitivity;


	bool m_bMoveForward;
	bool m_bMoveBack;
	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bMoveUp;
	bool m_bMoveDown;
	bool m_bRotationEnabled;
	bool m_bMovementEnabled;


};


#include "FPCamera.inl"


#endif //__FPCamera_H__