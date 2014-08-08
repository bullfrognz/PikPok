
//
//  File Name   :   Texture.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Texture_H__
#define __Texture_H__

// Dependent Includes
#include "Library/Buffer.h"

// Local Includes
#include "Defines/Macros.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes

class ITexture : public IBuffer
{

	// Member Types
public:

	enum EFormat
	{
		INVALID_FORMAT = -1,

		FORMAT_R16G16B16A16,
		FORMAT_R8G8B8A8,
		FORMAT_R16,
		FORMAT_R8,

		MAX_FORMAT = 4
	};

	enum EBindFlag
	{
		INVALID_BINDFLAG = -1,

		BINDFLAG_NONE			= 1	<< 0,
		BINDFLAG_SHADERRESOURCE = 1 << 1,
		BINDFLAG_STREAMOUTPUT	= 1 << 2,
		BINDFLAG_RENDERTARGET	= 1 << 3,
		BINDFLAG_DEPTHSTENCIL	= 1 << 4,

		MAX_BINDFLAG
	};

	struct TDesc
	{
		TDesc()
		{
			FW_MEMZERO(this, sizeof(TDesc));
		}

		c_char* kcpFile;

		EFormat		eFormat;
		EUsage		eUsage;
		EBindFlag	eBindFlag;
		ECPUAccess	eCPUAccess;

		uint uiWidth;		// Leave zero for auto detection when loading texture
		uint uiHeight;		// Leave zero for auto detection when loading texture
		uint uiNumMipMaps;	// Leave zero to automatically create mip maps
	};

	struct TTexelOffset
	{
		TTexelOffset()
		{
			FW_MEMZERO(this, sizeof(TTexelOffset));
		}

		uint uiTop;
		uint uiBottom;
		uint uiLeft;
		uint uiRight;
	};

	// Member Functions
public:

			 ITexture() {};
	virtual ~ITexture() {};

	virtual bool Initialise(const TDesc& _ktrDescription)																= 0;
	virtual bool UpdateRegion(void* _pTexelData, const TTexelOffset& _ktrOffsets, uint _uiRowPitch, uint _uiDepthPitch)	= 0;

	virtual bool Lock(ELockFlag _eLockFlag, void** _ppTextureData, uint& _uirRowPitch)	= 0;
	virtual void Unlock()																= 0;

	virtual uint	Width()			const = 0;
	virtual uint	Height()		const = 0;
	virtual EFormat Format()		const = 0;
	virtual uint	TexelSize()		const = 0;
	virtual uint	RowPitch()		const = 0;
	virtual uint	DepthPitch()	const = 0;

protected:

private:

	ITexture(const ITexture& _krTexture);
	ITexture& operator = (const ITexture& _krTexture);

	// Member Variables
protected:

private:

};

#endif //__Texture_H__