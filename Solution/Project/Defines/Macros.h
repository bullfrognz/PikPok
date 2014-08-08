
//
//  File Name   :   Macros.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Macros_H__
#define __Macros_H__

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <string.h>

// Implementation

#ifdef _DEBUG

#define FW_BREAKPOINT __debugbreak();

#else

#define FW_BREAKPOINT 0;

#endif

#define FW_REINTERPRET_CAST(SrcVariable, DestVaraible, Type) Type DestVaraible = reinterpret_cast<Type>(SrcVariable);


#define FW_MEMZERO(VARIABLE, SIZE)\
{\
	memset(VARIABLE, 0, SIZE); \
}


#define FW_MEMCOPY(DEST, SOURCE, SIZE)\
{\
	memcpy(DEST, SOURCE, SIZE); \
}


#define FW_MEMCOMPARE(SOURCE1, SOURCE2, SIZE) memcmp(SOURCE1, SOURCE2, SIZE)


#define FW_RELEASE(Variable)\
{\
if (Variable != 0)\
	{\
	Variable->Release(); \
	Variable = 0; \
	}\
}


#define FW_DELETE(Variable)\
{\
if (Variable != 0)\
	{\
	delete Variable; \
	Variable = 0; \
	}\
}


#define FW_ADELETE(ArrayVariable)\
{\
if (ArrayVariable != 0)\
	{\
	delete[] ArrayVariable; \
	ArrayVariable = 0; \
	}\
}


#define FW_DELETEMAP(KeyType, ValueType, Variable)\
{\
	std::map<KeyType, ValueType>::iterator __iterElement; \
	__iterElement = Variable.begin(); \
	\
while (__iterElement != Variable.end())\
	{\
	FW_DELETE((*__iterElement).second); \
	\
	\
	__iterElement = Variable.erase(__iterElement); \
	}\
}


#define FW_DELETEVECTOR(Type, Variable)\
{\
	std::vector<Type>::iterator __iterElement; \
	__iterElement = Variable.begin(); \
	\
while (__iterElement != Variable.end())\
	{\
	FW_DELETE((*__iterElement)); \
	\
	\
	__iterElement = Variable.erase(__iterElement); \
	}\
}


#define FW_DELETEARRAY(Variable, NumElements)\
{\
for (uint __iIndex = 0; __iIndex < NumElements; ++__iIndex)\
	{\
	FW_DELETE(Variable[__iIndex]); \
	}\
}

#endif //__Macros_H__