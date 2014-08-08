
//
//  File Name   :   ShapesScene.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __ShapesScene_H__
#define __ShapesScene_H__

// Dependent Includes
#include "Scenes/Scene.h"
#include "Library/Processable.h"
#include "Library/Observer.h"

// Local Includes
#include "Library/List.h"
#include "Library/JsonParser.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes
class CShape;
class IKeyboard;
class CCamera;
class IUiText;

class CShapesScene : public IScene, public CProcessable, public IObserver<IKeyboard>
{

	// Member Types
public:

	// Member Functions
public:

			 CShapesScene();
	virtual ~CShapesScene();

	virtual bool Initialise(void* _kpInitData);
	virtual void Process();

	virtual void Notify(IKeyboard& _rSender, short _sSubject, CEventArgs& _crArgs);

	void LoadSceneLayout(c_char* _kcpLayoutFilename);

protected:

private:

	void ParseAndCreateShape(CDictionary<std::string, CJsonParser::TValue>& _mrShapeProperties, uint _uiIndex);

	CShapesScene(const CShapesScene& _krShapesScene);
	CShapesScene& operator = (const CShapesScene& _krShapesScene);

	// Member Variables
protected:

private:

	CCamera*		m_pSceneCamera;
	IUiText*		m_pHelpText;

	CList<CShape*>	m_aShapes;

};

#endif //__ShapesScene_H__