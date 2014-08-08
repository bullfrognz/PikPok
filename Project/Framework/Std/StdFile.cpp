
//
//  File Name   :   StdFile.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "StdFile.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

CStdFile::CStdFile()
{
	// Empty
}

CStdFile::~CStdFile()
{
	Close();
}

bool CStdFile::Open(c_char* _kcpFile, uint _uiOpenAttribrutes)
{
	Close();

	uint uiOpenFlags = 0;

	if ((_uiOpenAttribrutes & OPEN_ATTRIBRUTE_BINARY) > 0)
		uiOpenFlags |= std::fstream::binary;

	if ((_uiOpenAttribrutes & OPEN_ATTRIBRUTE_READ) > 0)
		uiOpenFlags |= std::fstream::in;

	if ((_uiOpenAttribrutes & OPEN_ATTRIBRUTE_WRITE) > 0)
		uiOpenFlags |= std::fstream::out;

	if ((_uiOpenAttribrutes & OPEN_ATTRIBRUTE_CLEAR) > 0)
		uiOpenFlags |= std::fstream::trunc;

	if ((_uiOpenAttribrutes & OPEN_ATTRIBRUTE_OUTPUT_START_END) > 0)
		uiOpenFlags |= std::fstream::ate;

	if ((_uiOpenAttribrutes & OPEN_ATTRIBRUTE_APPEND_ONLY) > 0)
		uiOpenFlags |= std::fstream::app;

	m_FileStream.open(_kcpFile, uiOpenFlags);

	return (m_FileStream.is_open());
}

void CStdFile::Flush()
{
	if (!m_FileStream.is_open())
		return;

	m_FileStream.flush();
}

void CStdFile::Close()
{
	if (!m_FileStream.is_open())
		return;

	m_FileStream.close();
}

bool CStdFile::Read(char* _cpBuffer, uint _uiNumBytes)
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot read file. No file is opened.");
		return (false);
	}

	m_FileStream.read(_cpBuffer, _uiNumBytes);

	return (!m_FileStream.bad());
}

bool CStdFile::Write(c_char* _kcpData)
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot write to file. No file is opened.");
		return (false);
	}

	m_FileStream << _kcpData << std::endl;

	return (!m_FileStream.bad());
}

bool CStdFile::Write(c_char* _kcpData, uint _uiNumBytes)
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot write to file. No file is opened.");
		return (false);
	}

	m_FileStream.write(_kcpData, _uiNumBytes);
	m_FileStream << std::endl;

	return (!m_FileStream.bad());
}

void CStdFile::ReadPointer(uint _uiByteOffset)
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot set read pointer. No file is opened.");
		return;
	}

	m_FileStream.seekg(_uiByteOffset);
}

void CStdFile::WritePointer(uint _uiByteOffset)
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot set write pointer. No file is opened.");
		return;
	}

	m_FileStream.seekp(_uiByteOffset);
}

uint CStdFile::ReadPointer()
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot set read pointer. No file is opened.");
		return (0);
	}

	return ((uint)m_FileStream.tellg());
}

uint CStdFile::WritePointer()
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot set write pointer. No file is opened.");
		return (0);
	}

	return ((uint)m_FileStream.tellp());
}

uint CStdFile::Size()
{
	if (!m_FileStream.is_open())
	{
		DEBUG_ERROR("Cannot get file size. No file is opened.");
		return (0);
	}

	// Backup read position
	uint uiReadPosition = ReadPointer();

	// Move read pointer to end of file and get file length
	m_FileStream.seekg(0, m_FileStream.end);

	uint uiLength = ReadPointer();

	// Restore read position
	ReadPointer(uiReadPosition);

	return (uiLength);
}
