////////////////////////////////////////////////////////////////////////////////
// Filename: TextureSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextureSystem.h"

TextureSystem::TextureSystem()
{
	m_texture = 0;
}

TextureSystem::TextureSystem(const TextureSystem& other)
{
}

TextureSystem::~TextureSystem()
{
}

bool TextureSystem::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;


	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureSystem::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureSystem::GetTexture()
{
	return m_texture;
}
