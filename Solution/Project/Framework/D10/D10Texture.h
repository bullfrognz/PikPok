
//
//  File Name   :   D10Texture.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10Texture_H__
#define __D10Texture_H__

// Dependent Includes
#include "Library/Texture.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <DirectX/D3DX10.h>

// Prototypes
class CD10Renderer;

class CD10Texture : public ITexture
{

	// Member Types
public:

	// Member Functions
public:

			 CD10Texture(CD10Renderer& _rD10Renderer);
	virtual ~CD10Texture();

	virtual bool Initialise(const TDesc& _ktrDescription);
	virtual bool UpdateRegion(void* _pTexelData, const TTexelOffset& _ktrOffsets, uint _uiRowPitch, uint _uiDepthPitch);

	virtual bool Lock(ELockFlag _eLockFlag, void** _ppTextureData, uint& _uirRowPitch);
	virtual void Unlock();

	virtual uint	Width()			const;
	virtual uint	Height()		const;
	virtual EFormat Format()		const;
	virtual uint	TexelSize()		const;
	virtual uint	RowPitch()		const;
	virtual uint	DepthPitch()	const;

	ID3D10ShaderResourceView* GetResourceView();

protected:

	D3DX10_IMAGE_LOAD_INFO	CreateLoadInfo(const TDesc& _ktrDescription);
	D3D10_TEXTURE2D_DESC	CreateEmptyDesc(const TDesc& _ktrDescription);

	bool CreateTextureResource(D3DX10_IMAGE_LOAD_INFO& _trImageInfo);
	bool CreateShaderResourceView();
	bool Create2DTextureFromFile(D3DX10_IMAGE_LOAD_INFO& _trImageLoadInfo);
	bool Create2DTexture(D3D10_TEXTURE2D_DESC& _trD10TextureDese);

	void UpdateTexelSize();

private:

	bool InitialiseTexture(const TDesc& _ktrDescription);
	bool InitialiseResourceView(const TDesc& _ktrDescription);

	CD10Texture(const CD10Texture& _krD10Texture);
	CD10Texture& operator = (const CD10Texture& _krD10Texture);

	// Member Variables
protected:

private:

	CD10Renderer&				m_rD10Renderer;
	char*						m_cpFile;
	ID3D10Texture2D*			m_pTexture2D;
	ID3D10ShaderResourceView*	m_pResourceView;

	EFormat						m_eFormat;
	EUsage						m_eUsage;
	ECPUAccess					m_eCPUAccess;

	uint						m_uiWidth;
	uint						m_uiHeight;
	uint						m_uiTexelSize;
	uint						m_uiNumMipMaps;
	uint						m_uiBindFlags;
	uint						m_uiRowPitch;
	uint						m_uiDepthPitch;

	bool						m_bLocked;

};

#endif //__D10Texture_H__





















//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name   :   D10Texture.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef __D10Texture_H__
#define __D10Texture_H__


// Local Includes



// Library Includes



// Types


// Prototypes



class CD10Texture
{

	// Member Functions
public:





protected:





private:



	// Member Variables
protected:


private:





};


#include "D10Texture.inl"


#endif //__D10Texture_H__