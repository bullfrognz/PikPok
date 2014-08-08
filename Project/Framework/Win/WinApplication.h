
//
//  File Name   :   WinApplication.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __WinApplication_H__
#define __WinApplication_H__

// Dependent Includes
#include "Framework/Application.h"

// Local Includes
#include "Scenes/Scene.h"
#include "Defines/Types.h"

// Library Includes
#include <Windows.h>

// Prototypes
class CDebug;
class CWinClock;
class CWinWindow;
class CD10Renderer;
class CWinPointer;
class CWinKeyboard;	
class CStdFileSystem;
class IScene;	

class CWinApplication : public CApplication
{

	// Member Types
public:

	// Member Functions
public:

			 CWinApplication(CDebug* _pDebug);
	virtual ~CWinApplication();

	virtual void ExecuteOneFrame();
	virtual void Quit();

	virtual CDebug&			Debug();
	virtual IClock&			Clock();
	virtual IRenderer&		Renderer();
	virtual IWindow&		Window();
	virtual IPointer&		Pointer();
	virtual IKeyboard&		Keyboard();
	virtual IFileSystem&	FileSystem();

	bool Initialise(c_char* _kcpName, HINSTANCE& _hrInstance, uint _uiWindowWidth, uint _uiWindowHeight);

	HINSTANCE& GetInstanceHandle();

protected:

private:

	CWinApplication(const CWinApplication& _krWinApplication);
	CWinApplication& operator = (const CWinApplication& _krWinApplication);

	// Member Variables
protected:

private:

	HINSTANCE		m_hInstance;

	CDebug*			m_pDebug;
	CWinClock*		m_pClock;
	CD10Renderer*	m_pRenderer;
	CWinWindow*		m_pWindow;
	CWinPointer*	m_pPointer;
	CWinKeyboard*	m_pKeyboard;
	CStdFileSystem*	m_pFileSystem;

	char*			m_cpName;

};

#endif //__WinApplication_H__