////////////////////////////////////////////////////////////////////////////////
// Filename: ColorShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ColorShader_H_
#define _ColorShader_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <windows.h>
#include <xnamath.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShader
////////////////////////////////////////////////////////////////////////////////
class ColorShader
{
private:

	struct MatrixBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Update(ID3D11DeviceContext*, int, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
	ID3D11InputLayout*	m_pLayout;
	ID3D11Buffer*		m_pMatrixBuffer;
};

#endif
