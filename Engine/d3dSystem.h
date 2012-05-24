////////////////////////////////////////////////////////////////////////////////
// Filename: d3dSystem.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _d3dSystem_H_
#define _d3dSystem_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: d3dSystem
////////////////////////////////////////////////////////////////////////////////
class d3dSystem
{
public:

	d3dSystem();
	d3dSystem(const d3dSystem&);
	~d3dSystem();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

private:
	bool m_vsync_enabled;

	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3DXMATRIX m_mProjectionMatrix;
	D3DXMATRIX m_mWorldMatrix;
	D3DXMATRIX m_mOrthoMatrix;

};

#endif