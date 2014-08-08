
//
//  File Name   :   StrUtilities.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __StrUtilities_H__
#define __StrUtilities_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <string>

// Prototypes

namespace StrUtilities
{
	std::string		ExtractFilename(const std::string& _krFile);
	std::string		ExtractDirectoryPath(const std::string& _krFile);

	std::string		ConvertToString(int _iValue);
	std::string		ConvertToString(uint _uiValue);

	std::string&	ConvertToString(const std::wstring& _kwrInput, std::string& _rOutput);
	std::wstring&	ConvertToWString(const std::string& _krInput, std::wstring& _rwOutput);

	char*			ConvertToString(const wchar_t* _kwcpInput, char*& _cprOutput);
	wchar_t*		ConvertToWString(c_char* _kcpInput, wchar_t*& _wcprOutput);

	void			ConvertToInt(const std::string& _krString, int& _irReturnValue);
	void			ConvertToFloat(const std::string& _krString, float& _frReturnValue);

	std::string&	ToLower(std::string& _rString);

	char*			Copy(c_char* _kcpInput, char*& _prTarget);

	std::string&	Trim(std::string& _rString);
	std::string&	TrimRight(std::string& _rString);
	std::string&	TrimLeft(std::string& _rString);

	bool Compare(c_char* _kcpLeft, c_char* _kcpRight);

	std::string ToString(short _sValue);
	std::string ToString(ushort _usValue);

	std::string ToString(int _iValue);
	std::string ToString(uint _uiValue);

	std::string ToString(float _fValue);
	std::string ToString(double _dValue);

};

#endif //__StrUtilities_H__