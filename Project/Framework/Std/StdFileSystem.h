
//
//  File Name   :   StdFileSystem.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __StdFileSystem_H__
#define __StdFileSystem_H__

// Dependent Includes
#include "Framework/FileSystem.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

class CStdFileSystem : public IFileSystem
{

	// Member Types
public:

	// Member Functions
public:

			 CStdFileSystem();
	virtual ~CStdFileSystem();

	virtual IFile* InstanceFile();

	bool Initialise();

protected:

private:

	CStdFileSystem(const CStdFileSystem& _krStdFileSystem);
	CStdFileSystem& operator = (const CStdFileSystem& _krStdFileSystem);

	// Member Variables
protected:

private:

};

#endif //__StdFileSystem_H__