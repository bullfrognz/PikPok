
//
//  File Name   :   D10VertexBuffer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10VertexBuffer_H__
#define __D10VertexBuffer_H__

// Dependent Includes
#include "Library/VertexBuffer.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <d3d10.h>

// Prototypes
class CD10Renderer;

class CD10VertexBuffer : public IVertexBuffer
{

	// Member Types
public:

	// Member Functions
public:

			 CD10VertexBuffer(CD10Renderer& _rD10Renderer);
	virtual ~CD10VertexBuffer();

	virtual bool Initiailise(const TDesc& _ktrBufferDesc);
	virtual bool Draw(TMaterial& _rMaterial, uint _uiNumVertices, uint _uiOffset);

	virtual bool Lock(ELockFlag _eLockFlag, void** _ppBufferData);
	virtual void Unlock();

	virtual bool UpdateRegion(IRenderer& _rRenderer, void* _kpVertexData, uint _uiNumVertices, uint _uiIndexOffset);

	// Inline Functions
	virtual EPrimitiveType PrimitiveType() const;
	virtual EUsage BufferUsage() const;
	virtual ulong FvF() const;
	virtual uint Size() const;
	virtual uint Stride() const;
	virtual uint Capacity() const;

	ID3D10Buffer* GetBuffer();

protected:

	bool LockStatic(D3D10_MAP _eD10LockFlag, void** _ppVertexData);
	bool LockDynamic(D3D10_MAP _eD10LockFlag, void** _ppVertexData);

	bool ConvertLockFlag(ELockFlag _eLockFlag, D3D10_MAP& _erD10LockFlag);

private:

	CD10VertexBuffer(const CD10VertexBuffer& _krD10VertexBuffer);
	CD10VertexBuffer& operator = (const CD10VertexBuffer& _krD10VertexBuffer);

	// Member Variables
protected:

private:

	CD10Renderer&	m_rD10Renderer;

	ID3D10Buffer*	m_pVertexBuffer;

	EPrimitiveType	m_ePrimitiveType;
	EUsage			m_eUsage;

	ulong			m_ulFvF;

	uint			m_uiCapacity;
	uint			m_uiStride;

	bool			m_bLocked;

};

#endif //__D10VertexBuffer_H__