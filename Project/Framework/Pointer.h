
//
//  File Name   :   Pointer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Pointer_H__
#define __Pointer_H__

// Dependent Includes
#include "Library/Subject.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TVector2;

class IPointer : public CSubject<IPointer>
{

#define POINTER		APPLICATION.Pointer()

	// Member Types
public:

	enum ESubject
	{
		INVALID_SUBJECT,

		SUBJECT_STATE_CHANGE,
		SUBJECT_BUTTON_LEFT_STATE_CHANGE,
		SUBJECT_BUTTON_RIGHT_STATE_CHANGE,
		SUBJECT_BUTTON_MIDDLE_STATE_CHANGE,
		SUBJECT_BUTTON_WHEEL_UP,
		SUBJECT_BUTTON_WHEEL_DOWN,
		SUBJECT_MOVE,

		MAX_SUBJECT
	};

	enum EButton
	{
		INVALID_BUTTON,

		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_MIDDLE,
		BUTTON_WHEEL_UP,
		BUTTON_WHEEL_DOWN,

		MAX_BUTTON
	};

	enum EState
	{
		INVALID_STATE,

		STATE_UP,
		STATE_DOWN,
		STATE_PRESSED,
		STATE_RELEASED,

		MAX_STATE
	};

	// Member Functions
public:

			 IPointer() {};
	virtual ~IPointer() {};

	virtual void Center() = 0;

	virtual bool SetVisible(bool _bVisible) = 0;

	virtual const TVector2& GetPosition()										const = 0;
	virtual const TVector2& GetPreviousPosition()								const = 0;
	virtual EState			GetState()											const = 0;
	virtual EState			GetPreviousState()									const = 0;
	virtual EState			GetButtonState(EButton _eButton)					const = 0;
	virtual const TVector2& GetButtonUpPosition(EButton _eButton)				const = 0;
	virtual const TVector2& GetButtonDownPosition(EButton _eButton)				const = 0;
	virtual float			GetButtonDownDuration(EButton _eButton)				const = 0;
	virtual TVector2		GetSimultaneousTouchPosition(uchar _ucTouchIndex)	const = 0;
	virtual EState			GetSimultaneousTouchState(uint _ucTouchIndex)		const = 0;

	virtual bool IsStylusSupported()		const = 0;
	virtual bool IsMultiTouchSupported()	const = 0;
	virtual bool IsCursorVisible()			const = 0;

protected:

private:

	// Member Variables
protected:

private:

};

#endif //__Pointer_H__