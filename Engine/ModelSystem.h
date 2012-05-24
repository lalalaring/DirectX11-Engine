////////////////////////////////////////////////////////////////////////////////
// Filename: ModelSystem.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ModelSystem_H_
#define _ModelSystem_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "TextureSystem.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelSystem
////////////////////////////////////////////////////////////////////////////////
class ModelSystem
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelSystem();
	ModelSystem(const ModelSystem&);
	~ModelSystem();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;
	int m_iVertexCount, m_iIndexCount;
	TextureSystem* m_Texture;

	ModelType* m_model;
};

#endif
