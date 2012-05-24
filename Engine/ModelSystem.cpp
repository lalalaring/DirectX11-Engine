////////////////////////////////////////////////////////////////////////////////
// Filename: ModelSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ModelSystem.h"

ModelSystem::ModelSystem()
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
}

ModelSystem::ModelSystem(const ModelSystem& other)
{
}

ModelSystem::~ModelSystem()
{
}

bool ModelSystem::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	// Load in the model data,
	LoadModel(modelFilename);

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitializeBuffers(device);

	// Load the texture for this model.
	LoadTexture(device, textureFilename);

	return true;
}

void ModelSystem::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model texture.
	ReleaseTexture();

	// Release the model data.
	ReleaseModel();
	return;
}

void ModelSystem::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}

ID3D11ShaderResourceView* ModelSystem::GetTexture()
{
	return m_Texture->GetTexture();
}

int ModelSystem::GetIndexCount()
{
	return m_iIndexCount;
}

bool ModelSystem::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	/*
	// Set the number of vertices in the vertex array.
	m_iVertexCount = 4;

	// Set the number of indices in the index array.
	m_iIndexCount = 6;
	*/

	// Create the vertex array.
	vertices = new VertexType[m_iVertexCount];

	// Create the index array.
	indices = new unsigned long[m_iIndexCount];
	
	// Load the vertex array and index array with data.
	for(int i=0; i<m_iVertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	/*
	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// Load the index array with data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;  
	indices[4] = 3;  
	indices[5] = 0;
	*/
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;
	return true;
}

void ModelSystem::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}
	return;
}

void ModelSystem::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

bool ModelSystem::LoadTexture(ID3D11Device* device, WCHAR* filename)
{

	// Create the texture object.
	m_Texture = new TextureSystem;

	// Initialize the texture object.
	m_Texture->Initialize(device, filename);

	return true;
}

void ModelSystem::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool ModelSystem::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_iVertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_iIndexCount = m_iVertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_iVertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_iVertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void ModelSystem::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}
	return;
}

