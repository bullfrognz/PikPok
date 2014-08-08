
//
//  File Name   :   WinPointer.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "WinPointer.h"

// Local Includes
#include "Framework/Win/WinWindow.h"
#include "Framework/Debug.h"

// Library Includes
#include <WindowsX.h>

// Static Initialisers

// Defines
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

// Implementation

CWinPointer::CWinPointer(CWinWindow& _rWindow)
: m_rWindow(_rWindow)
, m_iTranslationX(0)
, m_iTranslationY(0)
, m_bCursorTmp(false)
, m_bCursorVisible(false)
, m_bWindowFocused(false)
{
	// Empty
}

CWinPointer::~CWinPointer()
{
	// Empty
}

void CWinPointer::Process()
{
	// Empty
}

void CWinPointer::Center()
{
	if (!WINDOW.IsFocused())
		return;

	TVector2 WindowDimentions((float)WINDOW.Width(),
		(float)WINDOW.Height());

	POINT PointerPosition;
	PointerPosition.x = static_cast<long>(WindowDimentions.x / 2) + (int)WINDOW.MarginX();
	PointerPosition.y = static_cast<long>(WindowDimentions.y / 2) + (int)WINDOW.MarginY();

	//::WindowToClient(static_cast<CWinWindow&>(WINDOW).GetHandle(), &PointerPosition);//Convert position relative to WINDOW

	::SetCursorPos(PointerPosition.x, PointerPosition.y);
}

bool CWinPointer::SetVisible(bool _bVisible)
{
	DEBUG_ERROR("The method or operation is not implemented.");
	
	return (false);
}

const TVector2& CWinPointer::GetPosition() const
{
	return (m_vWindowPosition);
}

const TVector2& CWinPointer::GetPreviousPosition() const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (m_vWindowPosition);
}

IPointer::EState CWinPointer::GetState() const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (INVALID_STATE);
}

IPointer::EState CWinPointer::GetPreviousState() const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (INVALID_STATE);
}

IPointer::EState CWinPointer::GetButtonState(EButton _eButton) const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (INVALID_STATE);
}

const TVector2& CWinPointer::GetButtonUpPosition(EButton _eButton) const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (m_vWindowPosition);
}

const TVector2& CWinPointer::GetButtonDownPosition(EButton _eButton) const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (m_vWindowPosition);
}

float CWinPointer::GetButtonDownDuration(EButton _eButton) const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (0.0f);
}

TVector2 CWinPointer::GetSimultaneousTouchPosition(uchar _ucTouchIndex) const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (TVector2());
}

IPointer::EState CWinPointer::GetSimultaneousTouchState(uint _ucTouchIndex) const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (INVALID_STATE);
}

bool CWinPointer::IsStylusSupported() const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (false);
}

bool CWinPointer::IsMultiTouchSupported() const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (false);
}

bool CWinPointer::IsCursorVisible() const
{
	DEBUG_ERROR("The method or operation is not implemented.");

	return (false);
}

bool CWinPointer::Initialise()
{
	InitialiseRawInput(m_rWindow.Handle());
	InitialiseSubjects(m_rWindow);

	return (true);
}

void CWinPointer::Notify(IWindow& _rSender, short _sSubject, CEventArgs& _crArgs)
{
	ESubject eSubject = IPointer::INVALID_SUBJECT;
	const CWinWindow::TPointerMsg* tpPointerMsg = reinterpret_cast<const CWinWindow::TPointerMsg*>(&_crArgs);

	switch (tpPointerMsg->uiMessage)
	{
	case WM_LBUTTONUP:
		eSubject = SUBJECT_BUTTON_LEFT_STATE_CHANGE;
		m_bButtonsDown[BUTTON_LEFT] = false;
		break;

	case WM_LBUTTONDOWN:
		eSubject = SUBJECT_BUTTON_LEFT_STATE_CHANGE;
		m_bButtonsDown[BUTTON_LEFT] = true;
		break;

	case WM_RBUTTONUP:
		eSubject = SUBJECT_BUTTON_RIGHT_STATE_CHANGE;
		m_bButtonsDown[BUTTON_RIGHT] = false;
		break;

	case WM_RBUTTONDOWN:
		eSubject = SUBJECT_BUTTON_RIGHT_STATE_CHANGE;
		m_bButtonsDown[BUTTON_RIGHT] = true;
		break;

	case WM_MOUSEMOVE:
		m_vWindowPosition.Set(static_cast<float>(GET_X_LPARAM(tpPointerMsg->lParam)),
			static_cast<float>(GET_Y_LPARAM(tpPointerMsg->lParam)));
		break;

	case WM_INPUT:
		eSubject = SUBJECT_MOVE;
		ReadRawInput(tpPointerMsg->lParam);
		break;

	case WM_MOUSEWHEEL:
		eSubject = (GET_WHEEL_DELTA_WPARAM(tpPointerMsg->wParam) > 0) ? SUBJECT_BUTTON_WHEEL_UP : SUBJECT_BUTTON_WHEEL_DOWN;
		break;
	}

	if (eSubject != INVALID_SUBJECT)
	{
		CSubject::NotifySubscribers(eSubject, 0);
	}
}

bool CWinPointer::InitialiseRawInput(HWND& _hrWindowHandle)
{
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage		= HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags		= RIDEV_INPUTSINK;
	Rid[0].hwndTarget	= _hrWindowHandle;

	::RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	return (true);
}

bool CWinPointer::InitialiseSubjects(CWinWindow& _rWinWindow)
{
	(_rWinWindow).Subscribe(this, CWinWindow::SUBJECT_MOUSE_MSG);


	return (true);
}

void CWinPointer::ReadRawInput(LPARAM _lParam)
{
	UINT dwSize = 40;
	static BYTE lpb[40];

	GetRawInputData(reinterpret_cast<HRAWINPUT>(_lParam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		m_iTranslationX += raw->data.mouse.lLastX;
		m_iTranslationY += raw->data.mouse.lLastY;
	}
}
