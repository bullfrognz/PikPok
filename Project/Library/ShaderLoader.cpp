
//
//  File Name   :   ShaderLoader.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "ShaderLoader.h"

// Local Includes
#include "Framework/Renderer.h"
#include "Framework/Debug.h"
#include "Library/Shader.h"

// Library Includes
#include <string>

// Static Initialisers

// Implementation

CShaderLoader::CShaderLoader()
: m_pShader(null)
{
	// Empty
}

CShaderLoader::~CShaderLoader()
{
	// Empty
}

IShader* CShaderLoader::Load(c_char* _kcpShaderAlias)
{
	CJsonParser JsonParser;
	if (JsonParser.ParseFile("Json/Shaders.json") == false)
	{
		DEBUG_ERROR("Failed to parse Shaders json file. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	CJsonParser::TValue tValue = JsonParser.BaseValue();

	if (!tValue.IsType(CJsonParser::VALUE_TYPE_OBJECT))
	{
		DEBUG_ERROR("Base value needs to be type object. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!tValue.AsObject().HasKey("Entries"))
	{
		DEBUG_ERROR("Missing 'Entries' key. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	tValue = tValue.AsObject()["Entries"];

	if (!tValue.IsType(CJsonParser::VALUE_TYPE_OBJECT))
	{
		DEBUG_ERROR("'Entires' expected to be of type Object. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!tValue.AsObject().HasKey(_kcpShaderAlias))
	{
		DEBUG_ERROR("Shader entry was not found. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	tValue = tValue.AsObject()[_kcpShaderAlias];

	return ( ParseAndInstanceShader(tValue, _kcpShaderAlias) );
}


IShader* CShaderLoader::ParseAndInstanceShader(CJsonParser::TValue& _trValue, c_char* _kcpShaderAlias)
{
	if (!_trValue.AsObject().HasKey("File"))
	{
		DEBUG_ERROR("Shader entry is missing 'File' key. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!_trValue.AsObject().HasKey("Technique"))
	{
		DEBUG_ERROR("Shader entry is missing 'Technique' key. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!_trValue.AsObject().HasKey("Variables"))
	{
		DEBUG_ERROR("Shader entry is missing 'Variables' key. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	CJsonParser::TValue tFile		= _trValue.AsObject()["File"];
	CJsonParser::TValue tTechnique	= _trValue.AsObject()["Technique"];
	CJsonParser::TValue tVariables	= _trValue.AsObject()["Variables"];

	if (!tFile.IsType(CJsonParser::VALUE_TYPE_STRING))
	{
		DEBUG_ERROR("Shader entry 'File' key type is invalid. Expecting string type. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!tTechnique.IsType(CJsonParser::VALUE_TYPE_OBJECT))
	{
		DEBUG_ERROR("Shader entry 'Technique' key type is invalid. Expecting object type. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!tVariables.IsType(CJsonParser::VALUE_TYPE_ARRAY))
	{
		DEBUG_ERROR("Shader entry 'Varaibles' key type is invalid. Expecting array type. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!tTechnique.AsObject().HasKey("Name"))
	{
		DEBUG_ERROR("Shader entry 'Technique' : 'Name' key missing. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	CJsonParser::TValue tTechniqueName = tTechnique.AsObject()["Name"];

	if (!tTechniqueName.IsType(CJsonParser::VALUE_TYPE_STRING))
	{
		DEBUG_ERROR("Shader entry 'Technique' : 'Name' key type is invalid. Expecting string type. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	if (!tTechnique.AsObject().HasKey("Inputs"))
	{
		DEBUG_ERROR("Shader entry 'Technique' : 'Inputs' key missing. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	CJsonParser::TValue tTechniqueInputs = tTechnique.AsObject()["Inputs"];

	if (!tTechniqueInputs.IsType(CJsonParser::VALUE_TYPE_ARRAY))
	{
		DEBUG_ERROR("Shader entry 'Technique' : 'Inputs' key type is invalid. Expecting array type. ShaderAlias(%s)", _kcpShaderAlias);
		return (null);
	}

	uint uiInputCount = tTechniqueInputs.AsArray().Count();
	IShader::EInput* epInputLayout = new IShader::EInput[uiInputCount];
	bool bInputsExtracted = true;

	for (uint i = 0; i < uiInputCount; ++ i)
	{
		CJsonParser::TValue tInputValue = tTechniqueInputs.AsArray()[i];

		if (!tInputValue.IsType(CJsonParser::VALUE_TYPE_STRING))
		{
			DEBUG_ERROR("Shader entry 'Technique' : 'Inputs' array element type is invalid. Expecting string type. ShaderAlias(%s) ArrayIndex(%d)", _kcpShaderAlias, i);
			return (null);
		}

		if (strcmp(tInputValue.AsString().c_str(), "POSITION3") == 0)
		{
			epInputLayout[i] = IShader::INPUT_POSITION3;
		}
		else if (strcmp(tInputValue.AsString().c_str(), "POSITION2") == 0)
		{
			epInputLayout[i] = IShader::INPUT_POSITION2;
		}
		else if (strcmp(tInputValue.AsString().c_str(), "NORMAL3") == 0)
		{
			epInputLayout[i] = IShader::INPUT_NORMAL3;
		}
		else if (strcmp(tInputValue.AsString().c_str(), "NORMAL2") == 0)
		{
			epInputLayout[i] = IShader::INPUT_NORMAL2;
		}
		else if (strcmp(tInputValue.AsString().c_str(), "TEXTURE_CORDS") == 0)
		{
			epInputLayout[i] = IShader::INPUT_TEXT_CORDS;
		}
		else
		{
			DEBUG_ERROR("Shader entry 'Technique' : 'Inputs' array element value is invalid. ShaderAlias(%s) ArrayIndex(%d) ArrayValue(%s)", _kcpShaderAlias, i, tInputValue.AsString().c_str());
			bInputsExtracted = false;
			break;
		}
	}

	IShader* m_pShader = null;

	if (bInputsExtracted)
	{
		std::string FilePath(SHADERS_DIR);
		FilePath += tFile.AsString();

		m_pShader = RENDERER.InstanceShader();

		m_pShader->LoadFile(FilePath.c_str(), IShader::INCLUDE_TRANSFORM | IShader::INCLUDE_LIGHTING | IShader::INCLUDE_TEXTURING);
		m_pShader->RegisterTechnique(tTechniqueName.AsString().c_str(), epInputLayout, uiInputCount);

		// Register variables
		for (uint i = 0; i < tVariables.AsArray().Count(); ++ i)
		{
			if (!tVariables.AsArray()[i].IsType(CJsonParser::VALUE_TYPE_STRING))
			{
				DEBUG_ERROR("Shader entry 'Variables' array element type is invalid. Expecting type string. ShaderAlias(%s) ArrayIndex(%d)", _kcpShaderAlias, i);
				continue;
			}

			m_pShader->RegisterVariable(tVariables.AsArray()[i].AsString().c_str());
		}
	}

	FW_ADELETE(epInputLayout);

	return (m_pShader);
}