
//
//  File Name   :   FileSystem.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __FileSystem_H__
#define __FileSystem_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
class IFile;

class IFileSystem
{

#define FILESYSTEM	APPLICATION.FileSystem()

	// Member Types
public:

	// Member Functions
public:

			 IFileSystem() {};
	virtual ~IFileSystem() {};

	virtual IFile* InstanceFile() = 0;

protected:

private:

	IFileSystem(const IFileSystem& _krFileSystem);
	IFileSystem& operator = (const IFileSystem& _krFileSystem);

	// Member Variables
protected:

private:

};

#endif //__FileSystem_H__