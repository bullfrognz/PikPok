
//
//  File Name   :   EventArgs.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __EventArgs_H__
#define __EventArgs_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <stdarg.h>

// Prototypes

class CEventArgs
{

	// Member Types
public:

	// Member Functions
public:

	CEventArgs(va_list _VarList)
	{
		m_VarList = _VarList;
	};

	~CEventArgs()
	{
		 // Empty
	};

	template <class T>
	T& GetNext()
	{
		return (va_arg(m_VarList, T));
	}

protected:

private:

	CEventArgs(const CEventArgs& _krEventArgs);
	CEventArgs& operator = (const CEventArgs& _krEventArgs);

	// Member Variables
protected:

private:

	va_list m_VarList;

};

#endif //__EventArgs_H__