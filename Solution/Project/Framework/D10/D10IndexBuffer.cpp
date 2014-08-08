
//
//  File Name   :   D10IndexBuffer.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "D10IndexBuffer.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/D10/D10Renderer.h"
#include "Framework/D10/D10VertexBuffer.h"
#include "Library/Material.h"

// Library Includes

// Static Initialisers

// Implementation

CD10IndexBuffer::CD10IndexBuffer(CD10Renderer& _rD10Renderer)
: m_rD10Renderer(_rD10Renderer)
, m_pIndexBuffer(0)
, m_eUsage(INVALID_USAGE)
, m_eIndexSize(INVALID_INDEXSIZE)
, m_uiCapacity(0)
, m_bLocked(0)
{
	// Empty
}

CD10IndexBuffer::~CD10IndexBuffer()
{
	FW_RELEASE(m_pIndexBuffer);
}

bool CD10IndexBuffer::Initiailise(const TDesc& _ktrBufferDesc)
{
	m_eUsage = _ktrBufferDesc.eUsage;
	m_eIndexSize = _ktrBufferDesc.eIndexSize;
	m_uiCapacity = _ktrBufferDesc.uiIndicesCount;

	bool bReturn = false;

	D3D10_BUFFER_DESC tIndexBufferDesc;
	FW_MEMZERO(&tIndexBufferDesc, sizeof(D3D10_BUFFER_DESC));
	tIndexBufferDesc.ByteWidth  = _ktrBufferDesc.uiIndicesCount;
	tIndexBufferDesc.ByteWidth *= (m_eIndexSize == INDEXSIZE_32) ? sizeof(int) : sizeof(short);
	tIndexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;

	switch (_ktrBufferDesc.eUsage)
	{
	case USAGE_STATIC:
		tIndexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		break;

	case USAGE_DYNAMIC:
		tIndexBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		tIndexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		break;

	case USAGE_STAGING:
		tIndexBufferDesc.Usage = D3D10_USAGE_STAGING;
		tIndexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		break;

	default:
		DEBUG_ERROR("Invalid usage flag");
		break;
	}

	D3D10_SUBRESOURCE_DATA tSubresourceDesc;
	FW_MEMZERO(&tSubresourceDesc, sizeof(D3D10_SUBRESOURCE_DATA));

	if (_ktrBufferDesc.kuipInitialData == 0)
	{
		tSubresourceDesc.pSysMem = new char[tIndexBufferDesc.ByteWidth];
	}
	else
	{
		tSubresourceDesc.pSysMem = _ktrBufferDesc.kuipInitialData;
	}
	
	static_cast<CD10Renderer&>(RENDERER).CreateBuffer(tIndexBufferDesc, tSubresourceDesc, m_pIndexBuffer);

	if (_ktrBufferDesc.kuipInitialData == 0)
	{
		FW_DELETE(tSubresourceDesc.pSysMem);
	}

	return (true);
}

bool CD10IndexBuffer::Draw(const TWorldMatrix& _ktrWorld, TMaterial& _rMaterial, IVertexBuffer* _pVertexBuffer, uint _uiNumIndices, uint _uiIndexOffset, uint _uiVertexOffset)
{
	VALIDATE(m_rD10Renderer.SetShader(_rMaterial.pShader));
	VALIDATE(m_rD10Renderer.SetVertexBuffer(_pVertexBuffer));
	VALIDATE(m_rD10Renderer.SetIndexBuffer(this));

	m_rD10Renderer.SetWorld(_ktrWorld);

	return (m_rD10Renderer.DrawIndexed(_uiNumIndices, _uiIndexOffset, _uiVertexOffset));
}

bool CD10IndexBuffer::Lock(ELockFlag _eLockFlag, void** _ppBufferData)
{
	DEBUG_ERROR_ON(!m_bLocked, "Buffer already locked");

	D3D10_MAP eLockFlag = D3D10_MAP_READ;
	bool bFlagDefined = true;
	bool bReturn = false;

	switch (_eLockFlag)
	{
	case LOCKFLAG_READONLY:
		eLockFlag = D3D10_MAP_READ;
		break;

	case LOCKFLAG_READWRITE:
		eLockFlag = D3D10_MAP_WRITE;
		break;

	case LOCKFLAG_WRITEONLY:
		eLockFlag = D3D10_MAP_READ_WRITE;
		break;

	case LOCKFLAG_WRITEDISCARD:
		eLockFlag = D3D10_MAP_WRITE_DISCARD;
		break;

	case LOCKFLAG_WRITENOOVERWRITE:
		eLockFlag = D3D10_MAP_WRITE_NO_OVERWRITE;
		break;

	default:
		bFlagDefined = false;
		break;
	}

	if (!bFlagDefined)
	{
		DEBUG_ERROR("Lock flag not supported");
	}
	else
	{
		HRESULT hLocked = m_pIndexBuffer->Map(eLockFlag, 0, _ppBufferData);


		if (FAILED(hLocked))
		{
			DEBUG_ERROR("Failed to lock");
		}
		else
		{
			bReturn = true;
			m_bLocked = true;
		}
	}

	return (bReturn);
}

void CD10IndexBuffer::Unlock()
{
	DEBUG_ERROR_ON(m_bLocked, "Buffer is not locked");

	m_pIndexBuffer->Unmap();
}

IIndexBuffer::EUsage CD10IndexBuffer::BufferUsage() const
{
	return (m_eUsage);
}

IIndexBuffer::EIndexSize CD10IndexBuffer::IndexSize() const
{
	return (m_eIndexSize);
}

uint CD10IndexBuffer::Capacity() const
{
	return (m_uiCapacity);
}

ID3D10Buffer* CD10IndexBuffer::D10Buffer()
{
	return (m_pIndexBuffer);
}