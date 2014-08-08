
//
//  File Name   :   WinWindow.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "WinWindow.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/StrUtilities.h"
#include "Resources/resource.h"

// Library Includes

// Static Initialisers

// Prototypes
LRESULT CALLBACK WindowProcWrapper(HWND _hWindow, unsigned int _uiMessage, WPARAM _wParam, LPARAM _lParam);

// Implementation

CWinWindow::CWinWindow(HINSTANCE& _hrInstance)
: m_hrInstandHandle(_hrInstance)
, m_hWindowHandle(null)
, m_cpTitle(null)
, m_ulStyles(0)
{
	// Empty
}

CWinWindow::~CWinWindow()
{
	FW_DELETE(m_cpTitle);
}

void CWinWindow::Title(c_char* _kcpTitle)
{
	::SetWindowText(m_hWindowHandle, _kcpTitle);
}

void CWinWindow::Dimentions(float _fWidth, float _fHeight)
{
	RECT tWindowRect;
	::GetWindowRect(m_hWindowHandle, &tWindowRect);

	::SetWindowPos(m_hWindowHandle, 0, tWindowRect.left, tWindowRect.top, (int)_fWidth, (int)_fHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void CWinWindow::Center()
{
	const HWND khDesktop = ::GetDesktopWindow();

	// Get screen rect
	RECT tScreenRect;
	::GetWindowRect(khDesktop, &tScreenRect);

	// Get window rect
	RECT tWindowRect;
	GetWindowRect(m_hWindowHandle, &tWindowRect);

	int iWindowHalfWidth  = (tWindowRect.right  - tWindowRect.left) / 2;
	int iWindowHalfHeight = (tWindowRect.bottom - tWindowRect.top)  / 2;

	// Move window
	::MoveWindow(m_hWindowHandle, 
				(tScreenRect.right / 2) - iWindowHalfWidth, (tScreenRect.bottom / 2) - iWindowHalfHeight, 
				(int)Width(), (int)Height(), false);
}

float CWinWindow::Width()
{
	RECT tInnerWindowRect;
	GetClientRect(m_hWindowHandle, &tInnerWindowRect);

	return ((float)(tInnerWindowRect.right - tInnerWindowRect.left));
}

float CWinWindow::Height()
{
	RECT tInnerWindowRect;
	GetClientRect(m_hWindowHandle, &tInnerWindowRect);

	return ((float)(tInnerWindowRect.bottom - tInnerWindowRect.top));
}

float CWinWindow::MarginX()
{
	RECT tWindowRect;
	::GetWindowRect(m_hWindowHandle, &tWindowRect);

	return ((float)tWindowRect.left);
}

float CWinWindow::MarginY()
{
	RECT tWindowRect;
	::GetWindowRect(m_hWindowHandle, &tWindowRect);

	return ((float)tWindowRect.top);
}

bool CWinWindow::IsFocused() 
{ 
	return (::GetActiveWindow() == m_hWindowHandle); 
}


bool CWinWindow::Initialise(c_char* _kcpTitle, ulong _ulStyles, uint _uiWidth, uint _uiHeight)
{
	StrUtilities::Copy(_kcpTitle, m_cpTitle);

	m_ulStyles = _ulStyles;

	VALIDATE(InitialiseWinClass());
	VALIDATE(InitialiseHandle(_uiWidth, _uiHeight));

	return (true);
}

LRESULT CALLBACK CWinWindow::Proc(HWND _hScreen, uint _uiMessage, WPARAM _wParam, LPARAM _lParam)
{
	CWinWindow::TProcMsg tStruct;
	tStruct.hWnd		= _hScreen;
	tStruct.uiMessage	= _uiMessage;
	tStruct.lParam		= _lParam;
	tStruct.wParam		= _wParam;

	CSubject::NotifySubscribers(CWinWindow::SUBJECT_PROC_MSG, &tStruct);

	switch (_uiMessage)
	{
	case WM_KILLFOCUS:
		CSubject::NotifySubscribers(SUBJECT_UNFOCUS);
		break;

	case WM_SETFOCUS:
		CSubject::NotifySubscribers(SUBJECT_FOCUS);
		break;

	case WM_CLOSE:
		CApplication::GetInstance().Quit();
		break;

	case WM_MOUSEWHEEL:
	case WM_INPUT:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		CWinWindow::TPointerMsg tMouseMsg;
		tMouseMsg.lParam = _lParam;
		tMouseMsg.wParam = _wParam;
		tMouseMsg.uiMessage = _uiMessage;

		CSubject::NotifySubscribers(SUBJECT_MOUSE_MSG, &tMouseMsg);

		break;
	}	

	case WM_KEYDOWN:
	{
		CWinWindow::TkeyboardMsg tKeyboardMsg;
		tKeyboardMsg.wParam = _wParam;
		tKeyboardMsg.bDown = true;

		CSubject::NotifySubscribers(SUBJECT_KEYBOARD_MSG, &tKeyboardMsg);

		break;
	}
			
	case WM_KEYUP:
	{
		CWinWindow::TkeyboardMsg tKeyboardMsg;
		tKeyboardMsg.wParam = _wParam;
		tKeyboardMsg.bDown = false;

		CSubject::NotifySubscribers(SUBJECT_KEYBOARD_MSG, &tKeyboardMsg);

		break;
	}

	default:
		break;
	}

	return (DefWindowProc(_hScreen, _uiMessage, _wParam, _lParam));
}


HWND& CWinWindow::Handle()
{
	return (m_hWindowHandle);
}

bool CWinWindow::InitialiseWinClass()
{
	WNDCLASSEX WinClass;
	WinClass.cbSize			= sizeof(WNDCLASSEX);
	WinClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; //CS_DBLCLKS | CS_OWNDC | 
	WinClass.lpfnWndProc	= WindowProcWrapper;
	WinClass.cbClsExtra		= 0;
	WinClass.cbWndExtra		= 0;
	WinClass.hInstance		= m_hrInstandHandle;
	WinClass.hIcon			= LoadIcon(m_hrInstandHandle, MAKEINTRESOURCE(APP_ICON));
	WinClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	WinClass.hbrBackground	= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	WinClass.lpszMenuName	= NULL;
	WinClass.lpszClassName	= m_cpTitle;//wcTitle
	WinClass.hIconSm		= LoadIcon(m_hrInstandHandle, MAKEINTRESOURCE(WINDOW_ICON));

	ATOM atomRegistered = RegisterClassEx(&WinClass);

	if (!atomRegistered)
	{
		DEBUG_ERROR("Failed to register win class");
		return (false);
	}

	return (true);
}

bool CWinWindow::InitialiseHandle(uint _uiWidth, uint _uiHeight)
{
	m_hWindowHandle = ::CreateWindowEx( NULL,
										m_cpTitle,//wcTitle
										m_cpTitle,//wcTitle
										m_ulStyles,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										_uiWidth,
										_uiHeight,
										NULL, NULL,
										m_hrInstandHandle,
										NULL);

	if (!m_hWindowHandle)
	{
		DEBUG_ERROR("Failed to create window handle");
		return (false);
	}

	return (true);
}

LRESULT CALLBACK WindowProcWrapper(HWND _hScreen, unsigned int _uiMessage, WPARAM _wParam, LPARAM _lParam)
{
	CWinWindow* pWinWindow = static_cast<CWinWindow*>(&CApplication::GetInstance().Window());

	if (pWinWindow != 0)
	{
		return (pWinWindow->Proc(_hScreen, _uiMessage, _wParam, _lParam));
	}
	else
	{
		return (DefWindowProc(_hScreen, _uiMessage, _wParam, _lParam));
	}
}