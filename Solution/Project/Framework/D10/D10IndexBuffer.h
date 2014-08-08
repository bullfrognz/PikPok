
//
//  File Name   :   D10IndexBuffer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10IndexBuffer_H__
#define __D10IndexBuffer_H__

// Dependent Includes
#include "Library/IndexBuffer.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <DirectX/D3DX10.h>

// Prototypes
class CD10Renderer;

class CD10IndexBuffer : public IIndexBuffer
{

	// Member Types
public:

	// Member Functions
public:

			 CD10IndexBuffer(CD10Renderer& _rD10Renderer);
	virtual ~CD10IndexBuffer();

	virtual bool Initiailise(const TDesc& _ktrBufferDesc);
	virtual bool Draw(const TWorldMatrix& _ktrWorld, TMaterial& _rMaterial, IVertexBuffer* _pVertexBuffer, uint _uiNumIndices, uint _uiIndexOffset, uint _uiVertexOffset);

	virtual bool Lock(ELockFlag _eLockFlag, void** _ppBufferData);
	virtual void Unlock();

	virtual EUsage		BufferUsage()	const;
	virtual EIndexSize	IndexSize()		const;
	virtual uint		Capacity()		const;

	ID3D10Buffer* D10Buffer();

protected:

private:

	CD10IndexBuffer(const CD10IndexBuffer& _krD10IndexBuffer);
	CD10IndexBuffer& operator = (const CD10IndexBuffer& _krD10IndexBuffer);

	// Member Variables
protected:

private:

	CD10Renderer&	m_rD10Renderer;

	ID3D10Buffer*	m_pIndexBuffer;

	EUsage			m_eUsage;
	EIndexSize		m_eIndexSize;

	uint			m_uiCapacity;

	bool			m_bLocked;

};

#endif //__D10IndexBuffer_H__