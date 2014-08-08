
//
//  File Name   :   IndexBuffer.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "IndexBuffer.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

IIndexBuffer::TDesc::TDesc()
: eUsage(INVALID_USAGE)
, eIndexSize(INDEXSIZE_32)
, kuipInitialData(0)
, uiIndicesCount(0)
{
	// Empty
}

IIndexBuffer::TDesc::TDesc(EUsage _eUsage, EIndexSize _eIndexSize, uint _uiIndicesCount, c_uint* _kuipInitialData)
: eUsage(_eUsage)
, eIndexSize(_eIndexSize)
, kuipInitialData(_kuipInitialData)
, uiIndicesCount(_uiIndicesCount)
{
	// Empty
}