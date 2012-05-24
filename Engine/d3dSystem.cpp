////////////////////////////////////////////////////////////////////////////////
// Filename: d3dSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "d3dSystem.h"

d3dSystem::d3dSystem()
{
	m_pSwapChain = 0;
	m_pDevice = 0;
	m_pDeviceContext = 0;
	m_pRenderTargetView = 0;
	m_pDepthStencilBuffer = 0;
	m_pDepthStencilState = 0;
	m_pDepthStencilView = 0;
}

d3dSystem::d3dSystem(const d3dSystem& other)
{
}

d3dSystem::~d3dSystem()
{
}

bool d3dSystem::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, 
						  float screenDepth, float screenNear)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_VIEWPORT viewport;
	UINT m4xMsaaQuality;
	bool mEnable4xMsaa = false;
	float fieldOfView, screenAspect;

////////////////////////////////////////////////////////////////////////////////

	D3D11CreateDevice(
			0,                 // default adapter
			D3D_DRIVER_TYPE_HARDWARE,
			0,                 // no software device
			0, 
			0, 0,              // default feature level array
			D3D11_SDK_VERSION,
			&m_pDevice,
			&featureLevel,
			&m_pDeviceContext);

	// Initialize the swap chain description.
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);

	if(m4xMsaaQuality > 0)
		mEnable4xMsaa = true;

	// Use 4X MSAA? 
	if( mEnable4xMsaa )
	{
		swapChainDesc.SampleDesc.Count   = 4;
		swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		swapChainDesc.SampleDesc.Count   = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	// Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
    swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
/*   swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;*/
	swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

////////////////////////////////////////////////////////////////////////////////
/*
	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
										   D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);

*/

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	      
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);

////////////////////////////////////////////////////////////////////////////////

	m_pSwapChain->ResizeBuffers(1, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// Get the pointer to the back buffer.
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

	// Create the render target view with the back buffer pointer.
	m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView);

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

////////////////////////////////////////////////////////////////////////////////

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if( mEnable4xMsaa )
	{
		depthBufferDesc.SampleDesc.Count   = 4;
		depthBufferDesc.SampleDesc.Quality = m4xMsaaQuality-1;
	}
	// No MSAA
	else
	{
		depthBufferDesc.SampleDesc.Count   = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);

	// Set the depth stencil state.
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);

////////////////////////////////////////////////////////////////////////////////

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

////////////////////////////////////////////////////////////////////////////////

	// Setup the viewport for rendering.
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

	// Create the viewport.
    m_pDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&m_mProjectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

    // Initialize the world matrix to the identity matrix.
    D3DXMatrixIdentity(&m_mWorldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&m_mOrthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void d3dSystem::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}

	if(m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = 0;
	}

	if(m_pDepthStencilState)
	{
		m_pDepthStencilState->Release();
		m_pDepthStencilState = 0;
	}

	if(m_pDepthStencilBuffer)
	{
		m_pDepthStencilBuffer->Release();
		m_pDepthStencilBuffer = 0;
	}

	if(m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = 0;
	}

	if(m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = 0;
	}

	if(m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = 0;
	}

	if(m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = 0;
	}
	return;
}

void d3dSystem::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];
	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
    
	// Clear the depth buffer.
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	return;
}

void d3dSystem::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_pSwapChain->Present(0, 0);
	}
	return;
}

ID3D11Device* d3dSystem::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext* d3dSystem::GetDeviceContext()
{
	return m_pDeviceContext;
}

void d3dSystem::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_mProjectionMatrix;
	return;
}

void d3dSystem::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_mWorldMatrix;
	return;
}

void d3dSystem::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_mOrthoMatrix;
	return;
}