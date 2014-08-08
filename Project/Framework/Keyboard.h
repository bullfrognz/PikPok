
//
//  File Name   :   Keyboard.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Keyboard_H__
#define __Keyboard_H__

// Dependent Includes
#include "Library/Subject.h"

// Local Includes

// Library Includes

// Prototypes

class IKeyboard : public CSubject<IKeyboard>
{

#define KEYBOARD	APPLICATION.Keyboard()

	// Member Types
public:

	enum ESubject
	{
		INVALID_SUBJECT,

		SUBJECT_KEY_STATE_CHANGE, // uchar

		MAX_SUBJECT
	};

	enum EKey
	{
		INVALID_KEY,

		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
		KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
		KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
		KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

		KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
		KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

		KEY_F1, KEY_F2, KEY_F3, KEY_F4,  KEY_F5,  KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

		KEY_SHIFT_L,   KEY_SHIFT_R,
		KEY_CONTROL_L, KEY_CONTROL_R,
		KEY_ALT_LEFT,  KEY_ALT_R,

		KEY_ARROW_U, KEY_ARROW_D,
		KEY_ARROW_L, KEY_ARROW_R,

		KEY_TAB,
		KEY_CAPSLOCK,
		KEY_SPACE,
		KEY_ESCAPE,
		KEY_BACKSPACE,
		KEY_ENTER,

		KEY_VOLUMNE_U, KEY_VOLUME_D,

		KEY_MAX
	};

	// Member Functions
public:

			 IKeyboard() {};
	virtual ~IKeyboard() {};

	virtual void SetOpened(bool _bOpen) = 0;

	virtual bool IsKeyDown(EKey _eKey)		= 0;
	virtual bool IsKeyPressed(EKey _eKey)	= 0;
	virtual bool IsKeyReleased(EKey _eKey)	= 0;
	virtual bool IsOpen()					= 0;

protected:

private:

	// Member Variables
protected:

private:

};

#endif //__Keyboard_H__