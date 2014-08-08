
//
//  File Name   :   Application.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Application_H__
#define __Application_H__

// Dependent Includes

// Local Includes
#include "Library/Subject.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes
class CDebug;
class IRenderer;
class IClock;
class IWindow;
class IPointer;
class IKeyboard;
class IScene;
class IFileSystem;

class CApplication : public CSubject<CApplication>
{

#define APPLICATION			CApplication::GetInstance()
#define SCENE(TYPE)			APPLICATION.GetScene<TYPE>()
#define ACTIVE_SCENE(TYPE)	APPLICATION.GetScene<TYPE>()

	// Member Types
public:

	enum ESubject
	{
		SUBJECT_INIT_COMPLETE,
		SUBJECT_QUIT_CALLED,
	};

	// Member Functions
public:

	virtual ~CApplication();

	virtual void ExecuteOneFrame() = 0;
	virtual void Quit();

	virtual CDebug&			Debug()			= 0;
	virtual IClock&			Clock()			= 0;
	virtual IRenderer&		Renderer()		= 0;
	virtual IWindow&		Window()		= 0;
	virtual IPointer&		Pointer()		= 0;
	virtual IKeyboard&		Keyboard()		= 0;
	virtual IFileSystem&	FileSystem()	= 0;

	virtual bool IsQuitting();

	template<class T>
	bool LoadScene(void* _pData = null)
	{
		m_pChangeToScene = new T;
		m_pChangeToSceneData = _pData;

		return (m_pChangeToScene != null);
	}

	IScene* GetCurrentScene();

	static CApplication& GetInstance();

protected:

	CApplication();

	void ExecuteSceneChange();

private:

	CApplication(const CApplication& _krApplication);
	CApplication& operator = (const CApplication& _krApplication);

	// Member Variables
protected:

	IScene*	m_pChangeToScene;
	void*	m_pChangeToSceneData;
	IScene* m_pCurrentScene;

	bool m_bQuit;

private:

	static CApplication* s_pInstance;

};

#endif //__Application_H__