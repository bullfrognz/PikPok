
//
//  File Name   :   D10Convert.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10Convert_H__
#define __D10Convert_H__

// Dependent Includes

// Local Includes
#include "Library/VertexBuffer.h"
#include "Library/Texture.h"
#include "Defines/Types.h"

// Library Includes
#include <DirectX/D3DX10.h>

// Prototypes

namespace D10Convert
{

	D3D10_PRIMITIVE_TOPOLOGY	PrimitiveType(IVertexBuffer::EPrimitiveType _eType);
	D3D10_USAGE					BufferUsage(IBuffer::EUsage _eUsage);
	uint						CPUAccess(IBuffer::ECPUAccess _eAccess);
	uint						BindFlags(ITexture::EBindFlag _eBindFlag);
	DXGI_FORMAT					TextureFormat(ITexture::EFormat _eFormat);
	D3D10_MAP					LockFlag(IBuffer::ELockFlag _eLockFlag);
	uint						TexelSize(ITexture::EFormat _eFormat);

}


#endif //__D10Convert_H__