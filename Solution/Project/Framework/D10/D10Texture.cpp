
//
//  File Name   :   D10Texture.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "D10Texture.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/D10/D10Renderer.h"
#include "Framework/D10/D10Convert.h"
#include "Library/StrUtilities.h"

// Library Includes

// Static Initialisers

// Implementation

CD10Texture::CD10Texture(CD10Renderer& _rD10Renderer)
: m_rD10Renderer(_rD10Renderer)
, m_cpFile(0)
, m_pTexture2D(0)
, m_pResourceView(0)
, m_eFormat(ITexture::INVALID_FORMAT)
, m_eUsage(IBuffer::INVALID_USAGE)
, m_eCPUAccess(IBuffer::INVALID_CPUACCESS)
, m_uiWidth(0)
, m_uiHeight(0)
, m_uiTexelSize(0)
, m_uiNumMipMaps(0)
, m_uiBindFlags(0)
, m_uiRowPitch(0)
, m_uiDepthPitch(0)
, m_bLocked(false)
{
	// Empty
}

CD10Texture::~CD10Texture()
{
	FW_RELEASE(m_pTexture2D);
	FW_RELEASE(m_pResourceView);
	FW_ADELETE(m_cpFile);
}

bool CD10Texture::Initialise(const TDesc& _ktrDescription)
{
	if (_ktrDescription.kcpFile != 0)
	{
		StrUtilities::Copy(_ktrDescription.kcpFile, m_cpFile);
	}

	m_eCPUAccess = _ktrDescription.eCPUAccess;
	m_eFormat = _ktrDescription.eFormat;
	m_eUsage = _ktrDescription.eUsage;
	m_uiBindFlags = _ktrDescription.eBindFlag;
	m_uiNumMipMaps = _ktrDescription.uiNumMipMaps;
	m_uiTexelSize = D10Convert::TexelSize(m_eFormat);

	VALIDATE(InitialiseTexture(_ktrDescription));
	VALIDATE(InitialiseResourceView(_ktrDescription));

	return (true);
}

bool CD10Texture::Lock(ELockFlag _eLockFlag, void** _ppTextureData, uint& _uirRowPitch)
{
	DEBUG_ERROR_ON(!m_bLocked, "Buffer is already locked");

	D3D10_MAP eD10LockFlag = D10Convert::LockFlag(_eLockFlag);
	bool bReturn = false;

	D3D10_MAPPED_TEXTURE2D tMappedTexture2D;
	HRESULT hLocked = m_pTexture2D->Map(D3D10CalcSubresource(0, 0, 1), eD10LockFlag, 0, &tMappedTexture2D);

	if (FAILED(hLocked))
	{
		DEBUG_ERROR("Failed to lock");
	}
	else
	{
		bReturn = true;
		m_bLocked = true;

		(*_ppTextureData) = tMappedTexture2D.pData;
		_uirRowPitch = tMappedTexture2D.RowPitch;
	}

	return (true);
}

void CD10Texture::Unlock()
{
	DEBUG_ERROR_ON(m_bLocked, "Buffer is not locked");

	m_pTexture2D->Unmap(0);
}

bool CD10Texture::UpdateRegion(void* _pTexelData, const TTexelOffset& _ktrOffsets, uint _uiRowPitch, uint _uiDepthPitch)
{
	D3D10_BOX tBox;
	FW_MEMZERO(&tBox, sizeof(D3D10_BOX));

	tBox.top = _ktrOffsets.uiTop;
	tBox.bottom = _ktrOffsets.uiBottom;

	tBox.front = 0;
	tBox.back = 1;

	tBox.left = _ktrOffsets.uiLeft;
	tBox.right = _ktrOffsets.uiRight;

	m_rD10Renderer.UpdateSubresource(m_pTexture2D, tBox, _pTexelData, _uiRowPitch, _uiDepthPitch);

	return (true);
}

D3DX10_IMAGE_LOAD_INFO CD10Texture::CreateLoadInfo(const TDesc& _ktrDescription)
{
	D3DX10_IMAGE_LOAD_INFO tD10ImageLoadInfo;
	FW_MEMZERO(&tD10ImageLoadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO));

	tD10ImageLoadInfo.Width				= _ktrDescription.uiWidth;
	tD10ImageLoadInfo.Height			= _ktrDescription.uiHeight;
	tD10ImageLoadInfo.Depth				= D3DX10_FROM_FILE;
	tD10ImageLoadInfo.MipLevels			= _ktrDescription.uiNumMipMaps;
	tD10ImageLoadInfo.BindFlags			= 0;
	tD10ImageLoadInfo.Filter			= D3DX10_FILTER_NONE;
	tD10ImageLoadInfo.MipFilter			= D3DX10_FILTER_NONE;
	tD10ImageLoadInfo.MipLevels			= m_uiNumMipMaps;

	tD10ImageLoadInfo.Usage				= D10Convert::BufferUsage(_ktrDescription.eUsage);
	tD10ImageLoadInfo.CpuAccessFlags	= D10Convert::CPUAccess(_ktrDescription.eCPUAccess);
	tD10ImageLoadInfo.BindFlags			= D10Convert::BindFlags(_ktrDescription.eBindFlag);
	tD10ImageLoadInfo.Format			= D10Convert::TextureFormat(_ktrDescription.eFormat);

	return (tD10ImageLoadInfo);
}

D3D10_TEXTURE2D_DESC CD10Texture::CreateEmptyDesc(const TDesc& _ktrDescription)
{
	D3D10_TEXTURE2D_DESC tD10Texture2DDesc;
	FW_MEMZERO(&tD10Texture2DDesc, sizeof(D3D10_TEXTURE2D_DESC));

	DXGI_SAMPLE_DESC tSampleDesc;
	tSampleDesc.Count					= 1;
	tSampleDesc.Quality					= 0;

	tD10Texture2DDesc.Width				= _ktrDescription.uiWidth;
	tD10Texture2DDesc.Height			= _ktrDescription.uiHeight;
	tD10Texture2DDesc.ArraySize			= 1;
	tD10Texture2DDesc.MipLevels			= _ktrDescription.uiNumMipMaps;
	//tD10Texture2DDesc.MiscFlags		= D3DX10_FILTER_NONE;
	tD10Texture2DDesc.SampleDesc		= tSampleDesc;

	tD10Texture2DDesc.Usage				= D10Convert::BufferUsage(_ktrDescription.eUsage);
	tD10Texture2DDesc.CPUAccessFlags	= D10Convert::CPUAccess(_ktrDescription.eCPUAccess);
	tD10Texture2DDesc.BindFlags			= D10Convert::BindFlags(_ktrDescription.eBindFlag);
	tD10Texture2DDesc.Format			= D10Convert::TextureFormat(_ktrDescription.eFormat);

	return (tD10Texture2DDesc);
}

bool CD10Texture::CreateTextureResource(D3DX10_IMAGE_LOAD_INFO& _trImageInfo)
{
	wchar_t* wcFile = 0;
	bool bReturn = false;

	HRESULT hResult = D3DX10CreateShaderResourceViewFromFile(m_rD10Renderer.GetDevice(), m_cpFile, &_trImageInfo, 0, &m_pResourceView, 0);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Failed to load texture");
	}
	else
	{
		bReturn = true;
	}

	return (bReturn);
}

bool CD10Texture::CreateShaderResourceView()
{
	D3D10_RESOURCE_DIMENSION eResourceType = D3D10_RESOURCE_DIMENSION_UNKNOWN;
	bool bReturn = false;

	m_pTexture2D->GetType(&eResourceType);

	if (eResourceType != D3D10_RESOURCE_DIMENSION_TEXTURE2D)
	{
		DEBUG_ERROR("Something else loaded instead of a 2d textre?");
	}
	else
	{
		D3D10_SHADER_RESOURCE_VIEW_DESC tResourceViewDesc;
		FW_MEMZERO(&tResourceViewDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));
		D3D10_TEXTURE2D_DESC tTexture2dDesc;
		FW_MEMZERO(&tTexture2dDesc, sizeof(D3D10_TEXTURE2D_DESC));

		ID3D10Texture2D* pD10Texture2D = static_cast<ID3D10Texture2D*>(m_pTexture2D);
		pD10Texture2D->GetDesc(&tTexture2dDesc);

		tResourceViewDesc.Format = tTexture2dDesc.Format;
		tResourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		tResourceViewDesc.Texture2D.MipLevels = tTexture2dDesc.MipLevels;
		tResourceViewDesc.Texture2D.MostDetailedMip = 0;

		HRESULT hResult = m_rD10Renderer.GetDevice()->CreateShaderResourceView(pD10Texture2D, &tResourceViewDesc, &m_pResourceView);

		if (FAILED(hResult))
		{
			DEBUG_ERROR("Failed to create shader resource view");
		}
		else
		{
			bReturn = true;
		}
	}

	return (bReturn);
}

bool CD10Texture::Create2DTextureFromFile(D3DX10_IMAGE_LOAD_INFO& _trImageLoadInfo)
{
	ID3D10Resource* pTextureResource = 0;
	wchar_t* wcFile = 0;
	bool bReturn = false;

	HRESULT hResult = D3DX10CreateTextureFromFile(m_rD10Renderer.GetDevice(), m_cpFile, &_trImageLoadInfo, 0, &pTextureResource, 0); //wcFile

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Texture failed to load");
	}
	else
	{
		m_pTexture2D = static_cast<ID3D10Texture2D*>(pTextureResource);
		bReturn = true;
	}

	return (bReturn);
}

bool CD10Texture::Create2DTexture(D3D10_TEXTURE2D_DESC& _trD10TextureDesc)
{
	bool bReturn = false;

	HRESULT hResult = m_rD10Renderer.GetDevice()->CreateTexture2D(&_trD10TextureDesc, NULL, &m_pTexture2D);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Failed to create empty texture");
	}
	else
	{
		bReturn = true;
	}

	return (true);
}

bool CD10Texture::InitialiseTexture(const TDesc& _ktrDescription)
{
	if (_ktrDescription.kcpFile == 0)
	{
		D3D10_TEXTURE2D_DESC tD10Texture2DDesc = CreateEmptyDesc(_ktrDescription);

		VALIDATE(Create2DTexture(tD10Texture2DDesc));
	}
	else
	{
		D3DX10_IMAGE_LOAD_INFO tD10ImageLoadInfo = CreateLoadInfo(_ktrDescription);

		VALIDATE(Create2DTextureFromFile(tD10ImageLoadInfo));
	}

	D3D10_TEXTURE2D_DESC tTexture2dDesc;
	m_pTexture2D->GetDesc(&tTexture2dDesc);

	m_uiWidth = tTexture2dDesc.Width;
	m_uiHeight = tTexture2dDesc.Height;
	m_uiRowPitch = m_uiTexelSize * m_uiWidth;
	m_uiDepthPitch = m_uiRowPitch * m_uiHeight;

	return (true);
}

bool CD10Texture::InitialiseResourceView(const TDesc& _ktrDescription)
{
	if (_ktrDescription.eBindFlag & BINDFLAG_SHADERRESOURCE)
	{
		VALIDATE(CreateShaderResourceView());
	}

	return (true);
}

uint CD10Texture::Width() const
{
	return (m_uiWidth);
}

uint CD10Texture::Height() const
{
	return (m_uiHeight);
}

ITexture::EFormat CD10Texture::Format() const
{
	return (m_eFormat);
}

uint CD10Texture::TexelSize() const
{
	return (m_uiTexelSize);
}

uint CD10Texture::RowPitch() const
{
	return (m_uiRowPitch);
}

uint CD10Texture::DepthPitch() const
{
	return (m_uiDepthPitch);
}

ID3D10ShaderResourceView* CD10Texture::GetResourceView()
{
	return (m_pResourceView);
}