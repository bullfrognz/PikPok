
//
//  File Name   :   D10VertexBuffer.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "D10VertexBuffer.h"

// Local Includes
#include "Framework/D10/D10Renderer.h"
#include "Framework/Debug.h"
#include "Library/Material.h"

// Library Includes

// Static Initialisers

// Implementation

CD10VertexBuffer::CD10VertexBuffer(CD10Renderer& _rD10Renderer)
: m_rD10Renderer(_rD10Renderer)
, m_pVertexBuffer(0)	
, m_ePrimitiveType(INVALID_PRIMITIVETYPE)
, m_eUsage(INVALID_USAGE)
, m_ulFvF(0)
, m_uiCapacity(0)
, m_uiStride(0)
, m_bLocked(false)
{
	// Empty
}

CD10VertexBuffer::~CD10VertexBuffer()
{
	FW_RELEASE(m_pVertexBuffer);
}

bool CD10VertexBuffer::Initiailise(const TDesc& _ktrBufferDesc)
{
	m_ePrimitiveType = _ktrBufferDesc.ePrimitiveType;
	m_eUsage = _ktrBufferDesc.eUsage;
	m_uiCapacity = _ktrBufferDesc.uiNumVertices;
	m_uiStride = _ktrBufferDesc.uiStride;

	bool bReturn = false;

	D3D10_BUFFER_DESC tVertexBufferDesc;
	FW_MEMZERO(&tVertexBufferDesc, sizeof(D3D10_BUFFER_DESC));
	tVertexBufferDesc.ByteWidth  = _ktrBufferDesc.uiNumVertices;
	tVertexBufferDesc.ByteWidth *= _ktrBufferDesc.uiStride;
	tVertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;

	switch (_ktrBufferDesc.eUsage)
	{
	case USAGE_STATIC:
		tVertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		break;

	case USAGE_DYNAMIC:
		tVertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		tVertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		break;

	case USAGE_STAGING:
		tVertexBufferDesc.Usage = D3D10_USAGE_STAGING;
		tVertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		break;

	default:
		DEBUG_ERROR("Invalid usage flag");
		break;
	}

	D3D10_SUBRESOURCE_DATA tSubresourceDesc;
	FW_MEMZERO(&tSubresourceDesc, sizeof(D3D10_SUBRESOURCE_DATA));

	if (_ktrBufferDesc.kpInitialData == 0)
	{
		tSubresourceDesc.pSysMem = new char[tVertexBufferDesc.ByteWidth];
	}
	else
	{
		tSubresourceDesc.pSysMem = _ktrBufferDesc.kpInitialData;
	}

	m_rD10Renderer.CreateBuffer(tVertexBufferDesc, tSubresourceDesc, m_pVertexBuffer);

	if (_ktrBufferDesc.kpInitialData == 0)
	{
		FW_DELETE(tSubresourceDesc.pSysMem);
	}

	return (true);
}

bool CD10VertexBuffer::Draw(TMaterial& _rMaterial, uint _uiNumVertices, uint _uiOffset)
{
	VALIDATE(m_rD10Renderer.SetShader(_rMaterial.pShader));
	VALIDATE(m_rD10Renderer.SetVertexBuffer(this));
	VALIDATE(m_rD10Renderer.SetIndexBuffer(null));

	return (m_rD10Renderer.Draw(_uiNumVertices, _uiOffset));
}

bool CD10VertexBuffer::Lock(ELockFlag _eLockFlag, void** _ppBufferData)
{
	DEBUG_ERROR_ON(m_bLocked, "Buffer is already locked");

	D3D10_MAP eD10LockFlag = D3D10_MAP_READ;
	bool bReturn = false;

	VALIDATE(ConvertLockFlag(_eLockFlag, eD10LockFlag));

	if (m_eUsage == USAGE_STATIC)
	{
		bReturn = LockStatic(eD10LockFlag, _ppBufferData);
	}
	else if (m_eUsage == USAGE_DYNAMIC)
	{
		bReturn = LockDynamic(eD10LockFlag, _ppBufferData);
	}
	else
	{
		DEBUG_ERROR("Buffer cannot be locked");
	}

	return (bReturn);
}

void CD10VertexBuffer::Unlock()
{
	DEBUG_ERROR_ON(!m_bLocked, "Buffer is not locked");

	m_pVertexBuffer->Unmap();
	m_bLocked = false;
}

bool CD10VertexBuffer::UpdateRegion(IRenderer& _rRenderer, void* _kpVertexData, uint _uiNumVertices, uint _uiIndexOffset)
{
	bool bReturn = false;

	CD10Renderer* pD10Renderer = dynamic_cast<CD10Renderer*>(&_rRenderer);

	if (pD10Renderer == 0)
	{
		DEBUG_ERROR("Invalid renderer type");
	}
	else
	{
		D3D10_BOX tBox;
		FW_MEMZERO(&tBox, sizeof(tBox));

		tBox.left = _uiIndexOffset * m_uiStride;
		tBox.right = tBox.left + (_uiNumVertices * m_uiStride);
		tBox.bottom = 1;
		tBox.back = 1;

		pD10Renderer->UpdateSubresource(m_pVertexBuffer, tBox, _kpVertexData, 0, 0);

		bReturn = true;
	}

	return (bReturn);
}

IVertexBuffer::EPrimitiveType CD10VertexBuffer::PrimitiveType() const
{
	return (m_ePrimitiveType);
}

IVertexBuffer::EUsage CD10VertexBuffer::BufferUsage() const
{
	return (m_eUsage);
}

ulong CD10VertexBuffer::FvF() const
{
	return (m_ulFvF);
}

uint CD10VertexBuffer::Size() const
{
	return (m_uiCapacity * m_uiStride);
}

uint CD10VertexBuffer::Stride() const
{
	return (m_uiStride);
}

uint CD10VertexBuffer::Capacity() const
{
	return (m_uiCapacity);
}

ID3D10Buffer* CD10VertexBuffer::GetBuffer()
{
	return (m_pVertexBuffer);
}

bool CD10VertexBuffer::LockStatic(D3D10_MAP _eD10LockFlag, void** _ppVertexData)
{
	HRESULT hResult = S_OK;
	bool bReturn = false;

	return (bReturn);
}

bool CD10VertexBuffer::LockDynamic(D3D10_MAP _eD10LockFlag, void** _ppVertexData)
{
	HRESULT hResult = S_OK;
	bool bReturn = false;

	hResult = m_pVertexBuffer->Map(_eD10LockFlag, 0, _ppVertexData);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Failed to lock");
	}
	else
	{
		bReturn = true;
		m_bLocked = true;
	}

	return (bReturn);
}

bool CD10VertexBuffer::ConvertLockFlag(ELockFlag _eLockFlag, D3D10_MAP& _erD10LockFlag)
{
	bool bReturn = true;

	switch (_eLockFlag)
	{
	case LOCKFLAG_READONLY:
		_erD10LockFlag = D3D10_MAP_READ;
		break;

	case LOCKFLAG_READWRITE:
		_erD10LockFlag = D3D10_MAP_READ_WRITE;
		break;

	case LOCKFLAG_WRITEONLY:
		_erD10LockFlag = D3D10_MAP_WRITE;
		break;

	case LOCKFLAG_WRITEDISCARD:
		_erD10LockFlag = D3D10_MAP_WRITE_DISCARD;
		break;

	case LOCKFLAG_WRITENOOVERWRITE:
		_erD10LockFlag = D3D10_MAP_WRITE_NO_OVERWRITE;
		break;

	default:
		bReturn = false;
		break;
	}

	return (bReturn);
}