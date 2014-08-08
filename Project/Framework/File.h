
//
//  File Name   :   File.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __File_H__
#define __File_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <string>

// Prototypes

class IFile
{

	// Member Types
public:

	enum EOpenAttribrute
	{
		INVALID_OPEN_ATTRIBRUTE,

		OPEN_ATTRIBRUTE_BINARY				= 1 << 0,
		OPEN_ATTRIBRUTE_READ				= 1 << 1,
		OPEN_ATTRIBRUTE_WRITE				= 1 << 2,
		OPEN_ATTRIBRUTE_CLEAR				= 1 << 3,
		OPEN_ATTRIBRUTE_OUTPUT_START_END	= 1 << 4,
		OPEN_ATTRIBRUTE_APPEND_ONLY			= 1 << 5,

		MAX_OPEN_ATTRIBRUTE
	};

	// Member Functions
public:

			 IFile() {};
	virtual ~IFile() {};

	virtual bool Open(c_char* _kcpFile, uint _uiOpenAttribrutes) = 0;
	virtual void Flush() = 0;
	virtual void Close() = 0;

	virtual bool Read(std::string& _rContents)				= 0;
	virtual bool Write(c_char* _kcpData)					= 0;
	virtual bool Write(c_char* _kcpData, uint _uiNumBytes)	= 0;

	virtual void ReadPointer(uint _uiByteOffset)	= 0;
	virtual void WritePointer(uint _uiByteOffset)	= 0;

	virtual uint ReadPointer()	= 0;
	virtual uint WritePointer()	= 0;

protected:

private:

	IFile(const IFile& _krFile);
	IFile& operator = (const IFile& _krFile);

	// Member Variables
protected:

private:

};

#endif //__File_H__