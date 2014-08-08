
//
//  File Name   :   Application.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Application.h"

// Local Includes
#include "Framework/Debug.h"
#include "Scenes/Scene.h"

// Library Includes

// Static Initialisers
CApplication* CApplication::s_pInstance = 0;

// Implementation

CApplication::CApplication()
: m_pChangeToScene(null)
, m_pChangeToSceneData(null)
, m_pCurrentScene(null)
, m_bQuit(false)
{
	s_pInstance = this;
}

CApplication::~CApplication()
{
	s_pInstance = 0;

	FW_DELETE(m_pCurrentScene);
}

void CApplication::Quit()
{
	m_bQuit = true;
}

bool CApplication::IsQuitting()
{
	return (m_bQuit);
}

IScene* CApplication::GetCurrentScene()
{
	return (m_pCurrentScene);
}

CApplication& CApplication::GetInstance()
{
	return (*s_pInstance);
}

void CApplication::ExecuteSceneChange()
{
	if (m_pChangeToScene == null)
		return;

	// Delete current scene
	FW_DELETE(m_pCurrentScene);

	// Set new current scene and initialise
	m_pCurrentScene = m_pChangeToScene;
	m_pCurrentScene->Initialise(m_pChangeToSceneData);

	// Cleanup
	m_pChangeToScene		= null;
	m_pChangeToSceneData	= null;
}