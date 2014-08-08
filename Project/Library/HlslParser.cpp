
//
//  File Name   :   HlslParser.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "HlslParser.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/FileSystem.h"
#include "Framework/File.h"

// Library Includes
#include <HLSL Parser/Engine/Allocator.h>
#include <HLSL Parser/M4HLSLParser.h>
#include <HLSL Parser/M4HLSLParser.h>

// Static Initialisers

// Implementation

CHlslParser::CHlslParser()
{
	// Empty
}

CHlslParser::~CHlslParser()
{
	// Empty
}

bool CHlslParser::Parse(c_char* _kcpFilename, c_char* _kcpShaderFilePath)
{
	IFile* pHlslFile = FILESYSTEM.InstanceFile();

	VALIDATE( pHlslFile->Open(_kcpShaderFilePath, IFile::OPEN_ATTRIBRUTE_READ) );

	uint uiFileSize = pHlslFile->Size() + 1;

	char* cFileContents = new char[uiFileSize];
	FW_MEMZERO(cFileContents, uiFileSize);

	pHlslFile->Read(cFileContents, uiFileSize);

	// Parse input file
	M4::Allocator Allocator;
	M4::HLSLParser Parser(&Allocator, _kcpFilename, cFileContents, uiFileSize);
	M4::HLSLTree Tree(&Allocator);

	if (!Parser.Parse(&Tree))
	{
		DEBUG_ERROR("Parsing failed, aborting");
	}

	FW_ADELETE(cFileContents);

	return (true);
}

bool GenerateGlsl(c_char* _kcpShaderFilePath)
{
	DEBUG_ERROR("Method not defined");
	return (false);

	/*
	// Generate output
	M4::GLSLGenerator generator(&allocator);
	generator.Generate(&tree, target, entryName);
	std::cout << generator.GetResult();
	*/
}