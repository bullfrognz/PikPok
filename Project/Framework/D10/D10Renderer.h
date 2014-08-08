
//
//  File Name   :   D10Renderer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10Renderer_H__
#define __D10Renderer_H__

// Dependent Includes
#include <Framework/Renderer.h>

// Local Includes
#include "Library/Shader.h"
#include "Library/WorldMatrix.h"
#include "Library/Colour.h"
#include "Defines/Types.h"

// Library Includes
#include <DirectX/D3DX10.h>

// Prototypes
class CD10Shader;
class CWinWindow;
class CCamera;
class ITexture;
class IVertexBuffer;
class IIndexBuffer;
class IUiText;
class CD10VertexBuffer;
class CD10IndexBuffer;
struct TColour;
struct TLight;

class CD10Renderer : public IRenderer
{

	// Member Types
public:

	enum EDeviceStates
	{
		DEVICESTATES_DEFAULT,
		DEVICESTATES_SHADOWDRAW,
		DEVICESTATES_SHADOWLITDRAW,

		MAX_DEVICESTATES
	};

	struct TActiveShader
	{
		CD10Shader* pD10Shader;
		ID3D10EffectTechnique* pTechnique;
		uint uiNumDrawPasses;
		uint uiStatesResetPassId;
	};

	struct TDeviceStates
	{
		ID3D10BlendState* m_pD10Blend;
		ID3D10RasterizerState* m_pD10Rasterizer;
		ID3D10DepthStencilState* m_pD10DepthStencil;
	};

	struct TActiveLight
	{
		TActiveLight()
		: kpLight(0)
		, bInUse(false)
		{
			// Empty
		}

		const TLight* kpLight;
		bool bInUse;
	};

	// Member Functions
public:

			 CD10Renderer(CWinWindow& _rWindow);
	virtual ~CD10Renderer();

	virtual void RenderFrame(bool _bClearTarget, bool _bClearDepth, bool _bClearStencil);

	virtual void AddLight(const TLight* _kpLight);
	virtual void RemoveLight(const TLight* _kpLight);

	virtual void Camera(CCamera* _pCamera);
	virtual void ClearColor(const TColour& _kvrClearColor);

	virtual uint			BufferWidth()	const;
	virtual uint			BufferHeight()	const;
	virtual CCamera&		Camera()		const;
	virtual const TColour&	ClearColor()	const;

	virtual IVertexBuffer*	InstanceVertexBuffer();
	virtual IIndexBuffer*	InstanceIndexBuffer();
	virtual ITexture*		InstanceTexture();
	virtual IShader*		InstanceShader();
	virtual IUiText*		InstanceUiText();

	bool Initialise(bool _bFullscreen);

	void Reset();
	void RestoreDefaultStates(uint _uiStates);
	bool DisableDepthBuffer();

	bool SetShader(IShader* _pShader);
	bool SetVertexBuffer(IVertexBuffer* _pVertexBuffer);
	bool SetIndexBuffer(IIndexBuffer* _pIndexBuffer);
	void SetWorld(const TWorldMatrix& _ktrWorld);

	bool Draw(uint _uiNumVertices, uint _uiOffset);
	bool DrawIndexed(uint _uiNumIndices, uint _uiIndexOffset, uint _uiVertexOffset);

	bool CreateBuffer(const D3D10_BUFFER_DESC& _krBufferDesc, const D3D10_SUBRESOURCE_DATA& _krSubresourceData, ID3D10Buffer*& _krBuffer);

	bool UpdateSubresource(ID3D10Resource* _pResource, const D3D10_BOX& _rBoundary, void* _pData, uint _uiRowPitch, uint _uiDepthPitch);

	ID3D10Device*	GetDevice();
	IDXGISwapChain* GetSwapChain();

protected:

	DXGI_SWAP_CHAIN_DESC CreateSwapChainDesc();
	D3D10_TEXTURE2D_DESC CreateTexture2DDesc();

private:

	bool InitialiseDirect3D();
	bool InitialiseAdapters();
	bool InitialiseDefaultStates();
	bool InitialiseDefaultCamera();

	void UpdateActiveShaderLights();
	void UpdateActiveShaderWorld();
	void UpdateActiveShaderTransforms();

	CD10Renderer(const CD10Renderer& _krD10Renderer);
	CD10Renderer& operator = (const CD10Renderer& _krD10Renderer);

	// Member Variables
protected:

private:

	CWinWindow&				m_rWindow;

	TActiveLight			m_tActiveLights[IShader::LIGHTS_MAX];
	TDeviceStates			m_tDeviceStates[MAX_DEVICESTATES];
	TActiveShader			m_tActiveShader;
	TWorldMatrix			m_tWorld;
	TColour					m_tClearColour;

	HWND					m_hWindowHandle;

	ID3D10Device*			m_pD3D10Device;
	IDXGISwapChain*			m_pSwapChain;
	ID3D10Texture2D*		m_pDepthStencilBuffer;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
	CCamera*				m_pActiveCamera;
	CCamera*				m_pDefaultCamera;

	D3D10_DRIVER_TYPE		m_eDriverType;

	uint					m_uiBufferWidth;
	uint					m_uiBufferHeight;

	bool					m_bFullscreen;
	bool					m_bCameraActivated;

	std::vector<IDXGIAdapter*> m_Adapters;

};

#endif //__D10Renderer_H__