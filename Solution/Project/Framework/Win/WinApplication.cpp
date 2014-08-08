
//
//  File Name   :   WinApplication.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "WinApplication.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Win/WinClock.h"
#include "Framework/D10/D10Renderer.h"
#include "Framework/Win/WinPointer.h"
#include "Framework/Win/WinKeyboard.h"
#include "Framework/Win/WinWindow.h"
#include "Framework/Std/StdFileSystem.h"
#include "Library/StrUtilities.h"

// Library Includes

// Static Initialisers

// Implementation

CWinApplication::CWinApplication(CDebug* _pDebug)
: m_hInstance(0)
, m_cpName(0)
, m_pClock(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_pPointer(0)
, m_pKeyboard(0)
, m_pFileSystem(0)
{
	m_pDebug = _pDebug;
}

CWinApplication::~CWinApplication()
{
	FW_DELETE(m_pCurrentScene);

	FW_DELETE(m_pKeyboard);
	FW_DELETE(m_pPointer);
	FW_DELETE(m_pClock);
	FW_DELETE(m_pWindow);
	FW_DELETE(m_pRenderer);
	FW_DELETE(m_pFileSystem);

	FW_ADELETE(m_cpName);
}

bool CWinApplication::Initialise(c_char* _kcpName, HINSTANCE& _hrInstance, uint _uiWindowWidth, uint _uiWindowHeight)
{
	StrUtilities::Copy(_kcpName, m_cpName);
	m_hInstance = _hrInstance;

	// Clock
	m_pClock = new CWinClock();
	VALIDATE(m_pClock->Initialise());

	// Window
	m_pWindow = new CWinWindow(_hrInstance);
	VALIDATE(m_pWindow->Initialise(_kcpName, WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU, _uiWindowWidth, _uiWindowHeight));

	// Renderer
	m_pRenderer = new CD10Renderer(*m_pWindow);
	VALIDATE(m_pRenderer->Initialise(false));

	// Keyboard
	m_pKeyboard = new CWinKeyboard(*m_pWindow);
	VALIDATE(m_pKeyboard->Initialise());

	// Pointer
	m_pPointer = new CWinPointer(*m_pWindow);
	VALIDATE(m_pPointer->Initialise());

	// File System
	m_pFileSystem = new CStdFileSystem();
	VALIDATE(m_pFileSystem->Initialise());

	return (true);
}

void CWinApplication::ExecuteOneFrame()
{
	m_pClock->ProcessFrame();
	m_pRenderer->RenderFrame(true, true, false);

	ExecuteSceneChange();
}

void CWinApplication::Quit()
{
	PostQuitMessage(0);
}

CDebug& CWinApplication::Debug()
{
	return (*m_pDebug);
}

IClock& CWinApplication::Clock()
{
	return (*m_pClock);
}

IRenderer& CWinApplication::Renderer()
{
	return (*m_pRenderer);
}

IWindow& CWinApplication::Window()
{
	return (*m_pWindow);
}

IPointer& CWinApplication::Pointer()
{
	return (*m_pPointer);
}

IKeyboard& CWinApplication::Keyboard()
{
	return (*m_pKeyboard);
}

IFileSystem& CWinApplication::FileSystem()
{
	return (*m_pFileSystem);
}

HINSTANCE& CWinApplication::GetInstanceHandle()
{
	return (m_hInstance);
}