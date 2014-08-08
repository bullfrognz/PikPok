
//
//  File Name   :   StdFile.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __StdFile_H__
#define __StdFile_H__

// Dependent Includes
#include "Framework/File.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <fstream>

// Prototypes

class CStdFile : public IFile
{

	// Member Types
public:

	// Member Functions
public:

			 CStdFile();
	virtual ~CStdFile();

	virtual bool Open(c_char* _kcpFile, uint _uiOpenAttribrutes);
	virtual void Flush();
	virtual void Close();

	virtual bool Read(std::string& _rContents);
	virtual bool Write(c_char* _kcpData);
	virtual bool Write(c_char* _kcpData, uint _uiNumBytes);

	virtual void ReadPointer(uint _uiByteOffset);
	virtual void WritePointer(uint _uiByteOffset);

	virtual uint ReadPointer();
	virtual uint WritePointer();

protected:

private:

	CStdFile(const CStdFile& _krStdFile);
	CStdFile& operator = (const CStdFile& _krStdFile);


	// Member Variables
protected:

private:

	std::fstream m_FileStream;

};

#endif //__StdFile_H__