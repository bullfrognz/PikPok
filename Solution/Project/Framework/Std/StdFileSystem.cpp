
//
//  File Name   :   StdFileSystem.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "StdFileSystem.h"

// Local Includes
#include "Framework/Std/StdFile.h"
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

CStdFileSystem::CStdFileSystem()
{
	// Empty
}

CStdFileSystem::~CStdFileSystem()
{
	// Empty
}

IFile* CStdFileSystem::InstanceFile()
{
	return (new CStdFile());
}

bool CStdFileSystem::Initialise()
{
	return (true);
}