
//
//  File Name   :   ShaderLoader.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __ShaderLoader_H__
#define __ShaderLoader_H__

// Dependent Includes

// Local Includes
#include "Library/JsonParser.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes
class IShader;
class CJsonParser;

class CShaderLoader
{

	// Member Types
public:

	// Member Functions
public:

	 CShaderLoader();
	~CShaderLoader();

	IShader* Load(c_char* _kcpShaderAlias);

protected:

private:

	IShader* ParseAndInstanceShader(CJsonParser::TValue& _trValue, c_char* _kcpShaderAlias);

	CShaderLoader(const CShaderLoader& _krShaderLoader);
	CShaderLoader& operator = (const CShaderLoader& _krShaderLoader);

	// Member Variables
protected:

private:

	IShader* m_pShader;

};

#endif //__ShaderLoader_H__