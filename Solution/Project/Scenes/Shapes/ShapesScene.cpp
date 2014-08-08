
//
//  File Name   :   ShapesScene.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "ShapesScene.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Keyboard.h"
#include "Framework/Renderer.h"
#include "Framework/Window.h"
#include "Library/Vector3.h"
#include "Library/Colour.h"
#include "Library/Camera.h"
#include "Library/UiText.h"
#include "Scenes/Shapes/Shape.h"
#include "Scenes/Shapes/PathMover.h"

// Library Includes

// Macros
#define VERIFY_SHAPE_DEFINITION_EXISTS(KEY)												\
if (!_mrShapeProperties.HasKey(KEY))													\
{																						\
	DEBUG_ERROR("Shape definition '" KEY "' is missing. ShapeIndex(%d)", _uiIndex);		\
	return;																				\
}

#define VERIFY_SHAPE_DEFINITION_VALUE_TYPE(KEY, JSON_VALUE_TYPE)																						\
if (!_mrShapeProperties[KEY].IsType(JSON_VALUE_TYPE))																									\
{																																						\
	DEBUG_ERROR("Shape '" KEY "' is invalid. Expecting '" #JSON_VALUE_TYPE "'. ShapeIndex(%d) JsonType(%d)", _uiIndex, _mrShapeProperties[KEY].eType);	\
	return;																																				\
}

// Static Initialisers

// Implementation

CShapesScene::CShapesScene()
: m_pSceneCamera(null)
, m_pHelpText(null)
{
	// Empty
}

CShapesScene::~CShapesScene()
{
	// Unsubscribe from key events
	KEYBOARD.Unsubscribe(this, IKeyboard::SUBJECT_KEY_STATE_CHANGE);

	// Delete instances
	FW_DELETE(m_pSceneCamera);
	FW_DELETE(m_pHelpText);

	for (uint i = 0; i < m_aShapes.Count(); ++i)
	{
		FW_DELETE(m_aShapes[i]);
	}
}

bool CShapesScene::Initialise(void* _kpInitData)
{
	// Sign up key events
	KEYBOARD.Subscribe(this, IKeyboard::SUBJECT_KEY_STATE_CHANGE);

	// Create scene camera
	m_pSceneCamera = new CCamera();
	m_pSceneCamera->Initialise(CCamera::DISPLAYTYPE_PERSPECTIVE, WINDOW.Width(), WINDOW.Height());
	m_pSceneCamera->World().RotateRadians(1.57f, 0.0f, 0.0f);
	m_pSceneCamera->World().Position(0.0f, 20.0f, 0.0f);
	
	RENDERER.Camera(m_pSceneCamera);

	// Create help text
	m_pHelpText = RENDERER.InstanceUiText();
	m_pHelpText->Text("Press 1 to view Layout1\nPress 2 to view Layout2\nPress ESC to Quit");

	return (true);
}

void CShapesScene::Process()
{
	// Empty
}

void CShapesScene::Notify(IKeyboard& _rSender, short _sSubject, CEventArgs& _crArgs)
{
	if (_sSubject == IKeyboard::SUBJECT_KEY_STATE_CHANGE)
	{
		IKeyboard::EKey eKey = _crArgs.GetNext<IKeyboard::EKey>();
		bool bDown		     = _crArgs.GetNext<bool>();

		switch (eKey)
		{
		case IKeyboard::KEY_ESCAPE:
			APPLICATION.Quit();
			break;

		case IKeyboard::KEY_1:
			LoadSceneLayout("Json/ShapesSceneLayout1.json");
			break;

		case IKeyboard::KEY_2:
			LoadSceneLayout("Json/ShapesSceneLayout2.json");
			break;
		}
	}
}

void CShapesScene::LoadSceneLayout(c_char* _kcpLayoutFilePath)
{
	CJsonParser JsonParser;
	JsonParser.ParseFile(_kcpLayoutFilePath);
	
	// Validate base value type
	CJsonParser::TValue tValue = JsonParser.BaseValue();

	if (!tValue.IsType(CJsonParser::VALUE_TYPE_OBJECT))
	{
		DEBUG_ERROR("Base value needs to be an object. LayoutFilePath(%s)", _kcpLayoutFilePath);
		return ;
	}

	// Validate shapes entries exists
	if (!tValue.AsObject().HasKey("Shapes"))
	{
		DEBUG_ERROR("'Shapes' key does not exists. LayoutFilePath(%s)", _kcpLayoutFilePath);
		return;
	}

	// Validate shapes entries are stored in an array
	tValue = tValue.AsObject()["Shapes"];

	if (!tValue.IsType(CJsonParser::VALUE_TYPE_ARRAY))
	{
		DEBUG_ERROR("'Shapes' value needs to be an array. LayoutFilePath(%s)", _kcpLayoutFilePath);
		return;
	}

	// Create each shape
	CList<CJsonParser::TValue>& arShapeEnties = tValue.AsArray();
	
	for (uint i = 0; i < arShapeEnties.Count(); ++ i)
	{
		tValue = arShapeEnties[i];

		// Validate shape entry is of type object
		if (!tValue.IsType(CJsonParser::VALUE_TYPE_OBJECT))
		{
			DEBUG_ERROR("Shape definition needs to be contained within an object. LayoutFilePath(%s)", _kcpLayoutFilePath);
			return;
		}

		ParseAndCreateShape(tValue.AsObject(), i);
	}

	// Delete unused shapes
	while (arShapeEnties.Count() < m_aShapes.Count())
	{
		FW_DELETE(m_aShapes[m_aShapes.Count() - 1]);
		m_aShapes.Remove(m_aShapes.Count() - 1);
	}
}

void CShapesScene::ParseAndCreateShape(CDictionary<std::string, CJsonParser::TValue>& _mrShapeProperties, uint _uiIndex)
{
	// Verify that required shape definitions exist
	VERIFY_SHAPE_DEFINITION_EXISTS("Model");
	VERIFY_SHAPE_DEFINITION_EXISTS("MovePattern");
	VERIFY_SHAPE_DEFINITION_EXISTS("Dimension");
	VERIFY_SHAPE_DEFINITION_EXISTS("MoveSpeed");
	VERIFY_SHAPE_DEFINITION_EXISTS("Colour");
	VERIFY_SHAPE_DEFINITION_EXISTS("StartPosition");

	// Verify shape definition values are valid
	VERIFY_SHAPE_DEFINITION_VALUE_TYPE("Model",			CJsonParser::VALUE_TYPE_STRING);
	VERIFY_SHAPE_DEFINITION_VALUE_TYPE("MovePattern",	CJsonParser::VALUE_TYPE_FIXED);
	VERIFY_SHAPE_DEFINITION_VALUE_TYPE("Dimension",		CJsonParser::VALUE_TYPE_REAL);
	VERIFY_SHAPE_DEFINITION_VALUE_TYPE("MoveSpeed",		CJsonParser::VALUE_TYPE_REAL);
	VERIFY_SHAPE_DEFINITION_VALUE_TYPE("Colour",		CJsonParser::VALUE_TYPE_ARRAY);
	VERIFY_SHAPE_DEFINITION_VALUE_TYPE("StartPosition", CJsonParser::VALUE_TYPE_ARRAY);

	// Get values as their types
	std::string	ShapeModel	= _mrShapeProperties["Model"].AsString();
	int	  iMovePattern		= _mrShapeProperties["MovePattern"].AsFixed();
	float fDimention		= (float)_mrShapeProperties["Dimension"].AsReal();
	float fMoveSpeed		= (float)_mrShapeProperties["MoveSpeed"].AsReal();
	CList<CJsonParser::TValue>& trColour		= _mrShapeProperties["Colour"].AsArray();
	CList<CJsonParser::TValue>& trStartPosition = _mrShapeProperties["StartPosition"].AsArray();

	TColour  tColour;
	TVector3 tStartPosition;

	// Validate move pattern
	if (iMovePattern <= CPathMover::INVALID_PATTERN ||
		iMovePattern >= CPathMover::MAX_PATTERN)
	{
		DEBUG_ERROR("Shape 'MovePattern' value is invalid. MinValue(%d) MaxValue(%d) SetValue(%d)", CPathMover::INVALID_PATTERN + 1, CPathMover::MAX_PATTERN - 1, iMovePattern);
		return;
	}

	CPathMover::EPattern eMovePattern = static_cast<CPathMover::EPattern>(iMovePattern);

	// Validate dimension
	if (fDimention <= 0.0f)
	{
		DEBUG_WARNING("Shape 'Dimension' value cannot be zero or lower. Setting to default value '1.0f'. Value(%d)", fDimention);
		
		fDimention = 1.0f;
	}

	// Validate move speed
	if (fMoveSpeed < 0.0f)
	{
		DEBUG_WARNING("Shape 'MoveSpeed' value cannot be lower then zero. Setting to default value '1.0f'. Value(%d)", fMoveSpeed);

		fMoveSpeed = 1.0f;
	}

	// Extract and validate colour
	if (trColour.Count() != 3)
	{
		DEBUG_WARNING("Shape 'Colour' value requires 3 real typed parameters. Setting to default value '[1.0f, 1.0f, 1.0f]'. Count(%d)", trColour.Count());
	}
	else
	{
		for (uint i = 0; i < trColour.Count(); ++i)
		{
			if (!trColour[i].IsType(CJsonParser::VALUE_TYPE_REAL))
			{
				DEBUG_WARNING("Shape 'Colour' element value is invalid. Expecting value type 'real'. Setting to default value '0.0f'. Index(%d)", i);

				tColour[i] = 0.0f;
			}
			else
			{
				tColour[i] = (float)trColour[i].AsReal();
			}
		}
	}

	// Extract and validate start position
	if (trStartPosition.Count() != 3)
	{
		DEBUG_WARNING("Shape 'StartPosition' value requires 3 real typed parameters. Setting to default value '[0.0f, 0.0f, 0.0f]'. Count(%d)", trStartPosition.Count());
	}
	else
	{
		for (uint i = 0; i < trStartPosition.Count(); ++ i)
		{
			if (!trStartPosition[i].IsType(CJsonParser::VALUE_TYPE_REAL))
			{
				DEBUG_WARNING("Shape 'StartPosition' element value is invalid. Expecting value type 'real'. Setting to default value '0.0f'. Index(%d)", i);

				tStartPosition[i] = 0.0f;
			}
			else
			{
				tStartPosition[i] = (float)trStartPosition[i].AsReal();
			}
		}
	}

	// Reuse old shapes one exists
	if (_uiIndex < m_aShapes.Count())
	{
		m_aShapes[_uiIndex]->Initialise(ShapeModel.c_str(), tColour, tStartPosition, eMovePattern, fDimention, fMoveSpeed);
	}
	else
	{
		// Instance shape type
		CShape* pNewShape = new CShape();
		pNewShape->Initialise(ShapeModel.c_str(), tColour, tStartPosition, eMovePattern, fDimention, fMoveSpeed);

		// Add shape to scene container
		m_aShapes.Add(pNewShape);
	}


}
