
//
//  File Name   :   D10Renderer.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "D10Renderer.h"

// Local Includes
#include "Framework/Win/WinWindow.h"
#include "Framework/D10/D10VertexBuffer.h"
#include "Framework/D10/D10IndexBuffer.h"
#include "Framework/D10/D10Texture.h"
#include "Framework/D10/D10Convert.h"
#include "Framework/D10/D10Shader.h"
#include "Framework/D10/D10UiText.h"
#include "Framework/Debug.h"
#include "Library/Vector3.h"
#include "Library/Renderable.h"

// Library Includes
#include "Library/Camera.h"

// Static Initialisers

// Implementation

CD10Renderer::CD10Renderer(CWinWindow& _rWindow)
: m_rWindow(_rWindow)
, m_pD3D10Device(0)
, m_pSwapChain(0)
, m_pDepthStencilBuffer(0)
, m_pRenderTargetView(0)
, m_pDepthStencilView(0)
, m_pActiveCamera(0)
, m_pDefaultCamera(0)
, m_eDriverType(D3D10_DRIVER_TYPE_HARDWARE)
, m_uiBufferWidth(0)
, m_uiBufferHeight(0)
, m_bFullscreen(false)
, m_bCameraActivated(false)
{
	FW_MEMZERO(&m_tActiveShader, sizeof(TActiveShader));
	FW_MEMZERO(m_tDeviceStates, sizeof(TDeviceStates)* MAX_DEVICESTATES);

	m_tClearColour.fR = 0.388f;
	m_tClearColour.fG = 0.803f;
	m_tClearColour.fB = 0.97f;
	m_tClearColour.fO = 0.0f;
}

CD10Renderer::~CD10Renderer()
{
	if (m_pD3D10Device != 0)
	{
		D3DX10UnsetAllDeviceObjects(m_pD3D10Device);
	}

	FW_RELEASE(m_pSwapChain);
	FW_RELEASE(m_pDepthStencilBuffer);
	FW_RELEASE(m_pDepthStencilView);
	FW_RELEASE(m_pRenderTargetView);

	for (uint i = 0; i < MAX_DEVICESTATES; ++i)
	{
		FW_RELEASE(m_tDeviceStates[i].m_pD10Blend);
		FW_RELEASE(m_tDeviceStates[i].m_pD10DepthStencil);
		FW_RELEASE(m_tDeviceStates[i].m_pD10Rasterizer);
	}

	FW_RELEASE(m_pD3D10Device);

	FW_DELETE(m_pDefaultCamera);
}

void CD10Renderer::RenderFrame(bool _bClearTarget, bool _bClearDepth, bool _bClearStencil)
{
	DEBUG_ERROR_ON(m_pActiveCamera == 0, "There is no activate camera for rendering.");

	m_pD3D10Device->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<float*>(&m_tClearColour));
	m_pD3D10Device->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	std::list<CRenderable*>& rRenderables = CRenderable::GetRenderablesList();
	std::list<CRenderable*>::iterator iterCurrentObject = rRenderables.begin();

	while (iterCurrentObject != rRenderables.end())
	{
		// Check rendering enabled
		if ((*iterCurrentObject)->RenderingEnabled())
		{
			// Render object
			(*iterCurrentObject)->Render();
		}

		++iterCurrentObject;
	}

	m_pSwapChain->Present(0, 0);

	// Reset states
	RestoreDefaultStates(STATE_BLEND | STATE_RASTERIZER | STATE_DEPTHSTENCIL);
}

void CD10Renderer::AddLight(const TLight* _kpLight)
{
	bool bAdded = false;

	for (uint i = 0; i < IShader::LIGHTS_MAX; ++i)
	{
		if (m_tActiveLights[i].bInUse == false)
		{
			m_tActiveLights[i].kpLight = _kpLight;
			m_tActiveLights[i].bInUse = true;

			bAdded = true;
			break;
		}
	}

	if (bAdded == false)
	{
		DEBUG_WARNING("Failed to add light. Maximum number reached.");
	}
	else
	{
		UpdateActiveShaderLights();
	}
}

void CD10Renderer::RemoveLight(const TLight* _kpLight)
{
	bool bRemoved = false;

	for (uint i = 0; i < IShader::LIGHTS_MAX; ++i)
	{
		if (m_tActiveLights[i].kpLight == _kpLight)
		{
			m_tActiveLights[i].kpLight = 0;
			m_tActiveLights[i].bInUse = false;

			bRemoved = true;
			break;
		}
	}

	if (bRemoved == false)
	{
		DEBUG_WARNING("Failed to remove light. Light was not activate.");
	}
	else
	{
		UpdateActiveShaderLights();
	}
}

void CD10Renderer::Camera(CCamera* _pCamera)
{
	if (_pCamera == null)
	{
		m_pActiveCamera = m_pDefaultCamera;
		return;
	}

	m_pActiveCamera = _pCamera;
}

void CD10Renderer::ClearColor(const TColour& _kvrClearColor)
{
	m_tClearColour = _kvrClearColor;
}

uint CD10Renderer::BufferWidth() const
{
	return (m_uiBufferWidth);
}

uint CD10Renderer::BufferHeight() const
{
	return (m_uiBufferHeight);
}

const TColour& CD10Renderer::ClearColor() const
{
	return (m_tClearColour);
}

CCamera& CD10Renderer::Camera() const
{
	return (*m_pActiveCamera);
}

IVertexBuffer* CD10Renderer::InstanceVertexBuffer()
{
	return (new CD10VertexBuffer(*this));
}

IIndexBuffer* CD10Renderer::InstanceIndexBuffer()
{
	return (new CD10IndexBuffer(*this));
}

ITexture* CD10Renderer::InstanceTexture()
{
	return (new CD10Texture(*this));
}

IShader* CD10Renderer::InstanceShader()
{
	return (new CD10Shader(*this));
}

IUiText* CD10Renderer::InstanceUiText()
{
	return (new CD10UiText(*this));
}

bool CD10Renderer::Initialise(bool _bFullscreen)
{
	m_hWindowHandle		= m_rWindow.Handle();
	m_uiBufferWidth		= (int)m_rWindow.Width();
	m_uiBufferHeight	= (int)m_rWindow.Height();
	m_bFullscreen		= _bFullscreen;

	VALIDATE(InitialiseDirect3D());
	VALIDATE(InitialiseAdapters());
	VALIDATE(InitialiseDefaultStates());
	VALIDATE(InitialiseDefaultCamera());

	return (true);
}

void CD10Renderer::Reset()
{
	bool bReturn = false;

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	FW_RELEASE(m_pDepthStencilBuffer);
	FW_RELEASE(m_pRenderTargetView);
	FW_RELEASE(m_pDepthStencilView);

	// Resize the swap chain and recreate the render target view.
	HRESULT hResult = m_pSwapChain->ResizeBuffers(1, m_uiBufferWidth, m_uiBufferHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Device or swap chain failed to create");
		return;
	}

	ID3D10Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	hResult = m_pD3D10Device->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Failed to create render target view");
		return;
	}

	FW_RELEASE(pBackBuffer);

	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC depthStencilDesc = CreateTexture2DDesc();
	m_pD3D10Device->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
	m_pD3D10Device->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView);

	// Bind the render target view and depth/stencil view to the pipeline.
	m_pD3D10Device->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Set the view port transform.
	D3D10_VIEWPORT tD10Viewport;
	tD10Viewport.TopLeftX = 0;
	tD10Viewport.TopLeftY = 0;
	tD10Viewport.Width = m_uiBufferWidth;
	tD10Viewport.Height = m_uiBufferHeight;
	tD10Viewport.MinDepth = 0.0f;
	tD10Viewport.MaxDepth = 1.0f;

	m_pD3D10Device->RSSetViewports(1, &tD10Viewport);
}

void CD10Renderer::RestoreDefaultStates(uint _uiStates)
{
	if (_uiStates & STATE_BLEND)
	{
		m_pD3D10Device->OMSetBlendState(m_tDeviceStates[DEVICESTATES_DEFAULT].m_pD10Blend, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}

	if (_uiStates & STATE_RASTERIZER)
	{
		m_pD3D10Device->RSSetState(m_tDeviceStates[DEVICESTATES_DEFAULT].m_pD10Rasterizer);
	}

	if (_uiStates & STATE_DEPTHSTENCIL)
	{
		m_pD3D10Device->OMSetDepthStencilState(m_tDeviceStates[DEVICESTATES_DEFAULT].m_pD10DepthStencil, 0);
	}
}

bool CD10Renderer::DisableDepthBuffer()
{
	D3D10_DEPTH_STENCIL_DESC tD10DepthStencilDesc;
	tD10DepthStencilDesc.DepthEnable = false;
	tD10DepthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;	// Stop writing to the depth buffer
	tD10DepthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;				// The shadows are still blocked by geometry
	tD10DepthStencilDesc.StencilEnable = false;	// Turn on the stencil buffer

	ID3D10DepthStencilState* pDepthStencilState;
	m_pD3D10Device->CreateDepthStencilState(&tD10DepthStencilDesc, &pDepthStencilState);
	m_pD3D10Device->OMSetDepthStencilState(pDepthStencilState, 0);

	return (true);
}

bool CD10Renderer::SetShader(IShader* _pShader)
{
	CD10Shader* pD10Shader = dynamic_cast<CD10Shader*>(_pShader);

	if (pD10Shader == null)
	{
		DEBUG_ERROR("Could not set shader. Instance type is invalid.");
		return (false);
	}

	if (pD10Shader != m_tActiveShader.pD10Shader)
	{
		CD10Shader::TTechniqueInfo& trTechniqueInfo = pD10Shader->GetTechniqueInfo();

		m_pD3D10Device->IASetInputLayout(trTechniqueInfo.pD10InputLayout);
		m_tActiveShader.pTechnique = trTechniqueInfo.pD10Technique;

		D3D10_TECHNIQUE_DESC tTechniqueDesc;
		trTechniqueInfo.pD10Technique->GetDesc(&tTechniqueDesc);
		m_tActiveShader.uiNumDrawPasses = tTechniqueDesc.Passes;

		m_tActiveShader.pD10Shader = pD10Shader;
	}

	UpdateActiveShaderTransforms();
	UpdateActiveShaderLights();

	return (true);
}

bool CD10Renderer::SetVertexBuffer(IVertexBuffer* _pVertexBuffer)
{
	CD10VertexBuffer* pD10VertexBuffer = dynamic_cast<CD10VertexBuffer*>(_pVertexBuffer);

	if (pD10VertexBuffer == null)
	{
		DEBUG_ERROR("Could not set vertex buffer. Instance type is invalid.");
		return (false);
	}

	D3D10_PRIMITIVE_TOPOLOGY eD10PrimitiveType = D10Convert::PrimitiveType(pD10VertexBuffer->PrimitiveType());
	ID3D10Buffer*	pD10Buffer = pD10VertexBuffer->GetBuffer();
	uint			uiStide = pD10VertexBuffer->Stride();
	uint			uiOffset = 0;

	m_pD3D10Device->IASetPrimitiveTopology(eD10PrimitiveType);
	m_pD3D10Device->IASetVertexBuffers(0, 1, &pD10Buffer, &uiStide, &uiOffset);

	return (true);
}

bool CD10Renderer::SetIndexBuffer(IIndexBuffer* _pIndexBuffer)
{
	CD10IndexBuffer* pD10IndexBuffer = dynamic_cast<CD10IndexBuffer*>(_pIndexBuffer);

	if (pD10IndexBuffer == null)
	{
		DEBUG_ERROR("Could not set vertex buffer. Instance type is invalid.");
		return (false);
	}

	m_pD3D10Device->IASetIndexBuffer(pD10IndexBuffer->D10Buffer(), pD10IndexBuffer->IndexSize() == IIndexBuffer::INDEXSIZE_32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);

	return (true);
}

void CD10Renderer::SetWorld(const TWorldMatrix& _ktrWorld)
{
	m_tWorld = _ktrWorld;
	UpdateActiveShaderWorld();
}

bool CD10Renderer::Draw(uint _uiNumVertices, uint _uiOffset)
{
	DEBUG_ERROR_ON(m_tActiveShader.pD10Shader == 0, "No shader has been activated.");

	for (uchar i = 0; i < m_tActiveShader.uiNumDrawPasses; ++i)
	{
		m_tActiveShader.pTechnique->GetPassByIndex(i)->Apply(0);

		m_pD3D10Device->Draw(_uiNumVertices, _uiOffset);
	}

	return (true);
}

bool CD10Renderer::DrawIndexed(uint _uiNumIndices, uint _uiIndexOffset, uint _uiVertexOffset)
{
	DEBUG_ERROR_ON(m_tActiveShader.pD10Shader == 0, "No shader has been activated.");

	for (uchar i = 0; i < m_tActiveShader.uiNumDrawPasses; ++i)
	{
		m_tActiveShader.pTechnique->GetPassByIndex(i)->Apply(0);

		m_pD3D10Device->DrawIndexed(_uiNumIndices, _uiIndexOffset, _uiVertexOffset);
	}

	return (true);
}

bool CD10Renderer::CreateBuffer(const D3D10_BUFFER_DESC& _krBufferDesc, const D3D10_SUBRESOURCE_DATA& _krSubresourceData, ID3D10Buffer*& _krBuffer)
{
	HRESULT hCreated = m_pD3D10Device->CreateBuffer(&_krBufferDesc, &_krSubresourceData, &_krBuffer);

	if (FAILED(hCreated))
	{
		DEBUG_ERROR("Failed to create buffer");
		return (false);
	}

	return (true);
}

bool CD10Renderer::UpdateSubresource(ID3D10Resource* _pResource, const D3D10_BOX& _rBoundary, void* _pData, uint _uiRowPitch, uint _uiDepthPitch)
{
	m_pD3D10Device->UpdateSubresource(_pResource, 0, &_rBoundary, _pData, _uiRowPitch, _uiDepthPitch);

	return (true);
}

ID3D10Device* CD10Renderer::GetDevice()
{
	return (m_pD3D10Device);
}

IDXGISwapChain* CD10Renderer::GetSwapChain()
{
	return (m_pSwapChain);
}

DXGI_SWAP_CHAIN_DESC CD10Renderer::CreateSwapChainDesc()
{
	DXGI_SWAP_CHAIN_DESC tSwapChainDesc;
	ZeroMemory(&tSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Refresh rate
	tSwapChainDesc.Windowed = (!m_bFullscreen);
	tSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	tSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// Back buffer
	tSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapChainDesc.BufferCount = 1;
	tSwapChainDesc.BufferDesc.Width = m_uiBufferWidth;
	tSwapChainDesc.BufferDesc.Height = m_uiBufferHeight;
	tSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// No multi sampling
	tSwapChainDesc.SampleDesc.Count = 1;
	tSwapChainDesc.SampleDesc.Quality = 0;

	tSwapChainDesc.OutputWindow = m_hWindowHandle;
	tSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	tSwapChainDesc.Flags = 0;

	return (tSwapChainDesc);
}

D3D10_TEXTURE2D_DESC CD10Renderer::CreateTexture2DDesc()
{
	D3D10_TEXTURE2D_DESC tDepthStencilDesc;
	ZeroMemory(&tDepthStencilDesc, sizeof(D3D10_TEXTURE2D_DESC));

	tDepthStencilDesc.Width = m_uiBufferWidth;
	tDepthStencilDesc.Height = m_uiBufferHeight;
	tDepthStencilDesc.MipLevels = 1;
	tDepthStencilDesc.ArraySize = 1;
	tDepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDepthStencilDesc.SampleDesc.Count = 1; // Multi sampling must match
	tDepthStencilDesc.SampleDesc.Quality = 0; // Swap chain values
	tDepthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
	tDepthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	tDepthStencilDesc.CPUAccessFlags = 0;
	tDepthStencilDesc.MiscFlags = 0;

	return (tDepthStencilDesc);
}

void CD10Renderer::UpdateActiveShaderLights()
{
	if ( m_tActiveShader.pD10Shader != null &&
		(m_tActiveShader.pD10Shader->Includes() & IShader::INCLUDE_LIGHTING) > 0)
	{ 
		// Lights
		const TLight* kpLights[IShader::LIGHTS_MAX];
		uint uiNumLights = 0;

		for (uint i = 0; i < IShader::LIGHTS_MAX; ++i)
		{
			if (m_tActiveLights[i].bInUse)
			{
				kpLights[uiNumLights] = m_tActiveLights[i].kpLight;

				++uiNumLights;
			}
		}

		m_tActiveShader.pD10Shader->SetLights(kpLights, uiNumLights, m_pActiveCamera->World().Position());
	}
}

void CD10Renderer::UpdateActiveShaderWorld()
{
	if (m_tActiveShader.pD10Shader != null)
	{
		m_tActiveShader.pD10Shader->SetTransformations(CD10Shader::TRANSFORM_WORLD, m_tWorld, m_pActiveCamera->View(), m_pActiveCamera->Projection());
	}
}

void CD10Renderer::UpdateActiveShaderTransforms()
{
	if ( m_tActiveShader.pD10Shader != null &&
		(m_tActiveShader.pD10Shader->Includes() & IShader::INCLUDE_TRANSFORM) > 0)
	{
		m_tActiveShader.pD10Shader->SetTransformations(CD10Shader::TRANSFORM_WORLD | CD10Shader::TRANSFORM_VIEW | CD10Shader::TRANSFORM_PROJECTION,
			m_tWorld, m_pActiveCamera->View(), m_pActiveCamera->Projection());
	}
}

bool CD10Renderer::InitialiseDirect3D()
{
	uint uiDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)

	//uiDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	//uiDeviceFlags |= D3D10_CREATE_DEVICE_SINGLETHREADED;

#endif

	HRESULT hResult = D3D10CreateDeviceAndSwapChain(0,                 // Default adapter
													m_eDriverType,
													0,                 // No software device
													uiDeviceFlags,
													D3D10_SDK_VERSION,
													&CreateSwapChainDesc(),
													&m_pSwapChain,
													&m_pD3D10Device);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Device or swap chain failed to create");
		return (false);
	}

	Reset();

	return (true);
}

bool CD10Renderer::InitialiseAdapters()
{
	IDXGIAdapter* pAdapter = null;
	IDXGIFactory* pFactory = null;

	HRESULT hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory));

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Failed to create DXGI factory.");
		return (false);
	}

	for (uint i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		m_Adapters.push_back(pAdapter);
	}

	FW_RELEASE(pFactory);

	return (true);
}

bool CD10Renderer::InitialiseDefaultStates()
{
	// Rasterizer
	D3D10_RASTERIZER_DESC tDefaultRasterizerDesc;
	tDefaultRasterizerDesc.FillMode = D3D10_FILL_SOLID;
	tDefaultRasterizerDesc.CullMode = D3D10_CULL_BACK;
	tDefaultRasterizerDesc.FrontCounterClockwise = false;
	tDefaultRasterizerDesc.DepthBias = 0;
	tDefaultRasterizerDesc.DepthBiasClamp = 0.0f;
	tDefaultRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	tDefaultRasterizerDesc.DepthClipEnable = true;
	tDefaultRasterizerDesc.ScissorEnable = false;
	tDefaultRasterizerDesc.MultisampleEnable = false;
	tDefaultRasterizerDesc.AntialiasedLineEnable = false;

	m_pD3D10Device->CreateRasterizerState(&tDefaultRasterizerDesc, &m_tDeviceStates[DEVICESTATES_DEFAULT].m_pD10Rasterizer);

	// Blend
	D3D10_BLEND_DESC tDefaultBlendDesc;
	tDefaultBlendDesc.AlphaToCoverageEnable = FALSE;
	tDefaultBlendDesc.SrcBlend = D3D10_BLEND_ONE;
	tDefaultBlendDesc.DestBlend = D3D10_BLEND_ZERO;
	tDefaultBlendDesc.BlendOp = D3D10_BLEND_OP_ADD;
	tDefaultBlendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	tDefaultBlendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	tDefaultBlendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;

	for (uint i = 0; i < 8; ++i)
	{
		tDefaultBlendDesc.BlendEnable[i] = false;
		tDefaultBlendDesc.RenderTargetWriteMask[i] = D3D10_COLOR_WRITE_ENABLE_ALL;
	}

	m_pD3D10Device->CreateBlendState(&tDefaultBlendDesc, &m_tDeviceStates[DEVICESTATES_DEFAULT].m_pD10Blend);

	// Depth & Stencil
	D3D10_DEPTH_STENCILOP_DESC tDefaultOpDesc;
	tDefaultOpDesc.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	tDefaultOpDesc.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
	tDefaultOpDesc.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	tDefaultOpDesc.StencilFunc = D3D10_COMPARISON_ALWAYS;

	D3D10_DEPTH_STENCIL_DESC tDepthStencilDesc;
	tDepthStencilDesc.DepthEnable = true;
	tDepthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	tDepthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
	tDepthStencilDesc.StencilEnable = false;
	tDepthStencilDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
	tDepthStencilDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;
	tDepthStencilDesc.FrontFace = tDefaultOpDesc;
	tDepthStencilDesc.BackFace = tDefaultOpDesc;

	m_pD3D10Device->CreateDepthStencilState(&tDepthStencilDesc, &m_tDeviceStates[DEVICESTATES_DEFAULT].m_pD10DepthStencil);

	return (true);
}

bool CD10Renderer::InitialiseDefaultCamera()
{
	m_pDefaultCamera = new CCamera();
	VALIDATE(m_pDefaultCamera->Initialise(CCamera::DISPLAYTYPE_PERSPECTIVE, (float)m_uiBufferWidth, (float)m_uiBufferHeight));
	m_pDefaultCamera->World().Position(0.0f, -10.0f, 2.0f);

	Camera(m_pDefaultCamera);

	return (true);
}