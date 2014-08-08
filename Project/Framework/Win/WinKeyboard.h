
//
//  File Name   :   WinKeyboard.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __WinKeyboard_H__
#define __WinKeyboard_H__

// Dependent Includes
#include "Framework/Keyboard.h"
#include "Library/Processable.h"
#include "Library/Observer.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
class IWindow;
class CWinWindow;

class CWinKeyboard : public IKeyboard, public CProcessable, public IObserver<IWindow>
{

	// Member Types
public:

	// Member Functions
public:

			 CWinKeyboard(CWinWindow& _rWindow);
	virtual ~CWinKeyboard();

	virtual void Process();

	virtual void SetOpened(bool _bOpen);

	virtual bool IsKeyDown(EKey _eKey);
	virtual bool IsKeyPressed(EKey _eKey);
	virtual bool IsKeyReleased(EKey _eKey);
	virtual bool IsOpen();

	bool Initialise();

protected:

	virtual void Notify(IWindow& _rSender, short _sSubject, CEventArgs& _rEventArgs);

private:

	CWinKeyboard(const CWinKeyboard& _krWinKeyboard);
	CWinKeyboard& operator = (const CWinKeyboard& _krWinKeyboard);

	// Member Variables
protected:

private:

	CWinWindow& m_rWindow;

};

#endif //__WinKeyboard_H__