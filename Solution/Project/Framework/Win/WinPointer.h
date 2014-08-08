
//
//  File Name   :   WinPointer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __WinPointer_H__
#define __WinPointer_H__

// Dependent Includes
#include "Framework/Pointer.h"
#include "Library/Processable.h"
#include "Library/Observer.h"

// Local Includes
#include "Library/Vector2.h"
#include "Defines/Types.h"

// Library Includes
#include <Windows.h>

// Prototypes
class CWinWindow;
class IWindow;

class CWinPointer : public IPointer, public CProcessable, public IObserver<IWindow>
{

	// Member Types
public:

	// Member Functions
public:

			  CWinPointer(CWinWindow& _rWindow);
	 virtual ~CWinPointer();

	 virtual void Process();
	 virtual void Center();

	 virtual bool SetVisible(bool _bVisible);

	 virtual const TVector2& GetPosition()										const;
	 virtual const TVector2& GetPreviousPosition()								const;
	 virtual EState			GetState()											const;
	 virtual EState			GetPreviousState()									const;
	 virtual EState			GetButtonState(EButton _eButton)					const;
	 virtual const TVector2& GetButtonUpPosition(EButton _eButton)				const;
	 virtual const TVector2& GetButtonDownPosition(EButton _eButton)			const;
	 virtual float			GetButtonDownDuration(EButton _eButton)				const;
	 virtual TVector2		GetSimultaneousTouchPosition(uchar _ucTouchIndex)	const;
	 virtual EState			GetSimultaneousTouchState(uint _ucTouchIndex)		const;

	 virtual bool IsStylusSupported()		const;
	 virtual bool IsMultiTouchSupported()	const;
	 virtual bool IsCursorVisible()			const;

	 virtual void Notify(IWindow& _rSender, short _sSubject, CEventArgs& _crArgs);

	 bool Initialise();

protected:

private:

	bool InitialiseRawInput(HWND& _hrWindowHandle);
	bool InitialiseSubjects(CWinWindow& _rWinWindow);

	void ReadRawInput(LPARAM _lParam);

	CWinPointer(const CWinPointer& _krWinPointer);
	CWinPointer& operator = (const CWinPointer& _krWinPointer);

	// Member Variables
protected:

private:

	CWinWindow& m_rWindow;

	TVector2 m_vWindowPosition;

	int m_iTranslationX;
	int m_iTranslationY;

	bool m_bCursorTmp;
	bool m_bCursorVisible;
	bool m_bWindowFocused;
	bool m_bButtonsDown[MAX_BUTTON];

};

#endif //__WinPointer_H__