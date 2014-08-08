
//
//  File Name   :   IndexBuffer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __IndexBuffer_H__
#define __IndexBuffer_H__

// Dependent Includes
#include "Library/Buffer.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
class IVertexBuffer;
struct TMaterial;
struct TWorldMatrix;

class IIndexBuffer : public IBuffer
{

	// Member Types
public:

	enum EIndexSize
	{
		INVALID_INDEXSIZE = -1,

		INDEXSIZE_16,
		INDEXSIZE_32,

		MAX_INDEXSIZE
	};

	struct TDesc
	{
		TDesc();
		TDesc(EUsage _eUsage, EIndexSize _eIndexSize, uint _uiIndicesCount, c_uint* _kuipInitialData = 0);

		EUsage eUsage;
		EIndexSize eIndexSize;
		c_uint* kuipInitialData;
		uint uiIndicesCount;
	};

	// Member Functions
public:

			 IIndexBuffer() {};
	virtual ~IIndexBuffer() {};

	virtual bool Initiailise(const TDesc& _ktrBufferDesc) = 0;
	virtual bool Draw(const TWorldMatrix& _ktrWorld, TMaterial& _rMaterial, IVertexBuffer* _pVertexBuffer, uint _uiNumIndices, uint _uiIndexOffset, uint _uiVertexOffset) = 0;

	virtual bool Lock(ELockFlag _eLockFlag, void** _ppBufferData)	= 0;
	virtual void Unlock()											= 0;

	virtual EUsage		BufferUsage()	const = 0;
	virtual EIndexSize	IndexSize()		const = 0;
	virtual uint		Capacity()		const = 0;

protected:

private:

	IIndexBuffer(const IIndexBuffer& _krIndexBuffer);
	IIndexBuffer& operator = (const IIndexBuffer& _krIndexBuffer);

	// Member Variables
protected:

private:

};

#endif //__IndexBuffer_H__