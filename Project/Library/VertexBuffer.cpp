
//
//  File Name   :   VertexBuffer.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "VertexBuffer.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

IVertexBuffer::TDesc::TDesc()
: kpInitialData(0)
, ePrimitiveType(INVALID_PRIMITIVETYPE)
, eUsage(INVALID_USAGE)
, uiNumVertices(0)
, uiStride(0)
{
	// Empty
}

IVertexBuffer::TDesc::TDesc(EUsage _eUsage, EPrimitiveType _ePrimitiveType, uint _uiNumVertices, uint _uiStride, c_void* _kpInitialData)
: kpInitialData(_kpInitialData)
, ePrimitiveType(_ePrimitiveType)
, eUsage(_eUsage)
, uiNumVertices(_uiNumVertices)
, uiStride(_uiStride)
{
	// Empty
}