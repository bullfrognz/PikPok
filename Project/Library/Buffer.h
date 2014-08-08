
//
//  File Name   :   Buffer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Buffer_H__
#define __Buffer_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

class IBuffer
{

	// Member Types
public:

	enum EUsage
	{
		INVALID_USAGE = -1,

		USAGE_STATIC,
		USAGE_DYNAMIC,
		USAGE_STAGING,

		MAX_USAGE
	};

	enum ELockFlag
	{
		INVALID_LOCKFLAG = -1,

		LOCKFLAG_READONLY = 1,
		LOCKFLAG_READWRITE = 1 << 1,
		LOCKFLAG_WRITEONLY = 1 << 2,
		LOCKFLAG_WRITEDISCARD = 1 << 3,
		LOCKFLAG_WRITENOOVERWRITE = 1 << 4,

		MAX_LOCKFLAG
	};

	enum ECPUAccess
	{
		INVALID_CPUACCESS = -1,

		CPUACCESS_NONE,
		CPUACCESS_WRITE,
		CPUACCESS_READ,
		CPUACCESS_READWRITE,

		MAX_CPUACCESS
	};

	// Member Functions
public:

			 IBuffer() {};
	virtual ~IBuffer() {};

protected:

private:

	IBuffer(const IBuffer& _krBuffer);
	IBuffer& operator = (const IBuffer& _krBuffer);

	// Member Variables
protected:

private:

};

#endif //__Buffer_H__