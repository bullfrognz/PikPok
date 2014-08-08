
//
//  File Name   :   Renderer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Renderer_H__
#define __Renderer_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TColour;
struct TProjectionMatrix;
struct TViewMatrix;
struct TWorldMatrix;
struct TLight;
struct TMaterial;
class ITexture;
class IShader;
class CCamera;
class IUiText;
class IVertexBuffer;
class IIndexBuffer;

class IRenderer
{

#define RENDERER APPLICATION.Renderer()

	// Member Types
public:

	enum EState
	{
		INVALID_STATE,

		STATE_BLEND			= 1 << 0,
		STATE_RASTERIZER	= 1 << 1,
		STATE_DEPTHSTENCIL	= 1 << 2,

		MAX_SATE
	};

	// Member Functions
public:

			 IRenderer() {};
	virtual ~IRenderer() {};

	virtual void RenderFrame(bool _bClearTarget, bool _bClearDepth, bool _bClearStencil) = 0;

	virtual void AddLight(const TLight* _kpLight)			= 0;
	virtual void RemoveLight(const TLight* _kpLight)		= 0;

	virtual void Camera(CCamera* _pCamera)					= 0;
	virtual void ClearColor(const TColour& _kvrClearColor)	= 0;

	virtual uint			BufferWidth()	const = 0;
	virtual uint			BufferHeight()	const = 0;
	virtual CCamera&		Camera()		const = 0;
	virtual const TColour&	ClearColor()	const = 0;

	virtual IVertexBuffer*	InstanceVertexBuffer()	= 0;
	virtual IIndexBuffer*	InstanceIndexBuffer()	= 0;
	virtual ITexture*		InstanceTexture()		= 0;
	virtual IShader*		InstanceShader()		= 0;
	virtual IUiText*		InstanceUiText()		= 0;

protected:

private:

	// Member Variables
protected:

private:

};

#endif //__Renderer_H__