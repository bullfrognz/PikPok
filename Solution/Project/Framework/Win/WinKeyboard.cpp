
//
//  File Name   :   WinKeyboard.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "WinKeyboard.h"

// Local Includes
#include "Framework/Win/WinWindow.h"
#include "Framework/Debug.h"

// Library Includes
#include <Windows.h>

// Static Initialisers

// Implementation

CWinKeyboard::CWinKeyboard(CWinWindow& _rWindow)
: m_rWindow(_rWindow)
{
	// Empty
}

CWinKeyboard::~CWinKeyboard()
{
	// Empty
}

void CWinKeyboard::Process()
{
	// Empty
}

void CWinKeyboard::SetOpened(bool _bOpen)
{
	DEBUG_WARNING("Method not supported");
}

bool CWinKeyboard::IsKeyDown(EKey _eKey)
{
	DEBUG_WARNING("Method not implemented");

	return (false);
}

bool CWinKeyboard::IsKeyPressed(EKey _eKey)
{
	DEBUG_WARNING("Method not implemented");

	return (false);
}

bool CWinKeyboard::IsKeyReleased(EKey _eKey)
{
	DEBUG_WARNING("Method not implemented");

	return (false);
}

bool CWinKeyboard::IsOpen()
{
	DEBUG_WARNING("Method not supported");

	return (false);
}

bool CWinKeyboard::Initialise()
{
	m_rWindow.Subscribe(this, CWinWindow::SUBJECT_KEYBOARD_MSG);

	return (true);
}

void CWinKeyboard::Notify(IWindow& _rSender, short _sSubject, CEventArgs& _rEventArgs)
{
	DEBUG_WARNING_ON(_sSubject != CWinWindow::SUBJECT_KEYBOARD_MSG, "Unwanted message recieved");

	const CWinWindow::TkeyboardMsg& ktrKeyboardMsg = *_rEventArgs.GetNext<CWinWindow::TkeyboardMsg*>();
	int iSubjectId = SUBJECT_KEY_STATE_CHANGE;
	EKey eKey = IKeyboard::INVALID_KEY;

	switch (ktrKeyboardMsg.wParam)
	{
	case VK_BACK:	 eKey = KEY_BACKSPACE;	break;
	case VK_TAB:	 eKey = KEY_TAB;		break;
	case VK_RETURN:	 eKey = KEY_ENTER;		break;
	case VK_SHIFT:	 eKey = KEY_SHIFT_L;	break;
	case VK_CONTROL: eKey = KEY_CONTROL_L;	break;
	case VK_ESCAPE:	 eKey = KEY_ESCAPE;		break;
	case VK_SPACE:	 eKey = KEY_SPACE;		break;
	case VK_LEFT:	 eKey = KEY_ARROW_L;	break;
	case VK_RIGHT:	 eKey = KEY_ARROW_R;	break;
	case VK_UP:		 eKey = KEY_ARROW_U;	break;
	case VK_DOWN:	 eKey = KEY_ARROW_D;	break;
	case VK_F1:		 eKey = KEY_F1;			break;
	case VK_F2:		 eKey = KEY_F2;			break;
	case VK_F3:		 eKey = KEY_F3;			break;
	case VK_F4:		 eKey = KEY_F4;			break;
	case VK_F5:		 eKey = KEY_F5;			break;
	case VK_F6:		 eKey = KEY_F6;			break;
	case VK_F7:		 eKey = KEY_F7;			break;
	case VK_F8:		 eKey = KEY_F8;			break;
	case VK_F9:		 eKey = KEY_F9;			break;
	case VK_F10:	 eKey = KEY_F10;		break;
	case VK_F11:	 eKey = KEY_F11;		break;
	case VK_F12:	 eKey = KEY_F12;		break;
	case 'A':		 eKey = KEY_A;			break;
	case 'B':		 eKey = KEY_B;			break;
	case 'C':		 eKey = KEY_C;			break;
	case 'D':		 eKey = KEY_D;			break;
	case 'E':		 eKey = KEY_E;			break;
	case 'F':		 eKey = KEY_F;			break;
	case 'G':		 eKey = KEY_G;			break;
	case 'H':		 eKey = KEY_H;			break;
	case 'I':		 eKey = KEY_I;			break;
	case 'J':		 eKey = KEY_J;			break;
	case 'K':		 eKey = KEY_K;			break;
	case 'L':		 eKey = KEY_L;			break;
	case 'M':		 eKey = KEY_M;			break;
	case 'N':		 eKey = KEY_N;			break;
	case 'O':		 eKey = KEY_O;			break;
	case 'P':		 eKey = KEY_P;			break;
	case 'Q':		 eKey = KEY_Q;			break;
	case 'R':		 eKey = KEY_R;			break;
	case 'S':		 eKey = KEY_S;			break;
	case 'T':		 eKey = KEY_T;			break;
	case 'U':		 eKey = KEY_U;			break;
	case 'V':		 eKey = KEY_V;			break;
	case 'W':		 eKey = KEY_W;			break;
	case 'X':		 eKey = KEY_X;			break;
	case 'Y':		 eKey = KEY_Y;			break;
	case 'Z':		 eKey = KEY_Z;			break;
	case '1':		 eKey = KEY_1;			break;
	case '2':		 eKey = KEY_2;			break;
	case '3':		 eKey = KEY_3;			break;
	case '4':		 eKey = KEY_4;			break;
	case '5':		 eKey = KEY_5;			break;
	case '6':		 eKey = KEY_6;			break;
	case '7':		 eKey = KEY_7;			break;
	case '8':		 eKey = KEY_8;			break;
	case '9':		 eKey = KEY_9;			break;
	case '0':		 eKey = KEY_0;			break;
	}

	if (eKey > IKeyboard::INVALID_KEY)
		CSubject::NotifySubscribers(SUBJECT_KEY_STATE_CHANGE, eKey, ktrKeyboardMsg.bDown);
}