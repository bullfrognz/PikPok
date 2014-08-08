
//
//  File Name   :   VertexBuffer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __VertexBuffer_H__
#define __VertexBuffer_H__

// Dependent Includes
#include "Library/Buffer.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
class IRenderer;
struct TMaterial;

class IVertexBuffer : public IBuffer
{

	// Member Types
public:

	enum EPrimitiveType
	{
		INVALID_PRIMITIVETYPE = -1,

		PRIMITIVETYPE_POINTLIST,
		PRIMITIVETYPE_LINELIST,
		PRIMITIVETYPE_LINESTRIP,
		PRIMITIVETYPE_TRIANGLELIST,
		PRIMITIVETYPE_TRIANGLESTRIP,

		MAX_PRIMITIVETYPE
	};

	struct TDesc
	{
		TDesc();
		TDesc(EUsage _eUsage, EPrimitiveType _ePrimitiveType, uint _uiNumVertices, uint _uiStride, c_void* _kpInitialData = 0);

		c_void* kpInitialData;

		EPrimitiveType ePrimitiveType;
		EUsage eUsage;

		uint uiNumVertices;
		uint uiStride;
	};

	// Member Functions
public:

			 IVertexBuffer() {};
	virtual ~IVertexBuffer() {};

	virtual bool Initiailise(const TDesc& _ktrBufferDesc)															= 0;
	virtual bool Draw(TMaterial& _rMaterial, uint _uiNumVertices, uint _uiOffset)															= 0;
	virtual bool UpdateRegion(IRenderer& _rRenderer, void* _kpVertexData, uint _uiNumVertices, uint _uiIndexOffset) = 0;

	virtual bool Lock(ELockFlag _eLockFlag, void** _ppBufferData)	= 0;
	virtual void Unlock()											= 0;

	virtual EPrimitiveType	PrimitiveType()	const = 0;
	virtual EUsage			BufferUsage()	const = 0;
	virtual ulong			FvF()			const = 0;
	virtual uint			Size()			const = 0;
	virtual uint			Stride()		const = 0;
	virtual uint			Capacity()		const = 0;

protected:

private:

	IVertexBuffer(const IVertexBuffer& _krVertexBuffer);
	IVertexBuffer& operator = (const IVertexBuffer& _krVertexBuffer);

	// Member Variables
protected:

private:

};

#endif //__VertexBuffer_H__