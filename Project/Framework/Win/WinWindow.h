
//
//  File Name   :   WinWindow.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __WinWindow_H__
#define __WinWindow_H__

// Dependent Includes
#include "Framework/Window.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <Windows.h>

// Prototypes

class CWinWindow : public IWindow
{

	// Member Types
public:

	enum ESubject
	{
		SUBJECT_KEYBOARD_MSG = IWindow::MAX_SUBJECT,	// TPointerMsg*
		SUBJECT_MOUSE_MSG,								// TkeyboardMsg*
		SUBJECT_PROC_MSG,								// TProcMsg*

		MaxSubject
	};

	struct TProcMsg
	{
		HWND hWnd;
		uint uiMessage;
		LPARAM lParam;
		WPARAM wParam;
	};

	struct TPointerMsg
	{
		uint uiMessage;
		LPARAM lParam;
		WPARAM wParam;
	};

	struct TkeyboardMsg
	{
		WPARAM wParam;
		bool bDown;
	};

	// Member Functions
public:

			 CWinWindow(HINSTANCE& _hrInstance);
	virtual ~CWinWindow();

	virtual void Title(c_char* _kcpTitle);
	virtual void Dimentions(float _fWidth, float _fHeight);
	virtual void Center();

	virtual float Width();
	virtual float Height();
	virtual float MarginX();
	virtual float MarginY();

	virtual bool IsFocused();

	bool				Initialise(c_char* _kcpTitle, ulong _ulStyles, uint _uiWidth, uint _uiHeight);
	LRESULT CALLBACK	Proc(HWND _hScreen, uint _uiMessage, WPARAM _wParam, LPARAM _lParam);

	HWND& Handle();

protected:

private:

	bool InitialiseWinClass();
	bool InitialiseHandle(uint _uiWidth, uint _uiHeight);

	CWinWindow(const CWinWindow& _krWinWindow);
	CWinWindow& operator = (const CWinWindow& _krWinWindow);

	// Member Variables
protected:

private:

	HINSTANCE&	m_hrInstandHandle;
	HWND		m_hWindowHandle;

	char*		m_cpTitle;

	ulong		m_ulStyles;

};

#endif //__WinWindow_H__