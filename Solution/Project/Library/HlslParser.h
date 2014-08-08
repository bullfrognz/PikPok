
//
//  File Name   :   HlslParser.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __HlslParser_H__
#define __HlslParser_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

class CHlslParser
{

	// Member Types
public:

	// Member Functions
public:

	 CHlslParser();
	~CHlslParser();

	bool Parse(c_char* _kcpFilename, c_char* _kcpShaderFilePath);
	bool GenerateGlsl(c_char* _kcpShaderFilePath);

protected:

private:

	CHlslParser(const CHlslParser& _krHlslParser);
	CHlslParser& operator = (const CHlslParser& _krHlslParser);

	// Member Variables
protected:

private:

};

#endif //__HlslParser_H__