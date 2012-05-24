////////////////////////////////////////////////////////////////////////////////
// Filename: GraphicSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "GraphicSystem.h"

GraphicSystem::GraphicSystem()
{
	m_pD3D = 0;
	m_pCamera = 0;
	m_pModel = 0;
	// m_pColorShader = 0;
	// m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
}

GraphicSystem::GraphicSystem(const GraphicSystem& other)
{
}

GraphicSystem::~GraphicSystem()
{
}

bool GraphicSystem::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Create the Direct3D object.
	m_pD3D = new d3dSystem;
	// Initialize the Direct3D object.
	m_pD3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	// Create the camera object.
	m_pCamera = new CameraSystem;
	// Set the initial position of the camera.
	m_pCamera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	m_pModel = new ModelSystem;
	// Initialize the model object.
	m_pModel->Initialize(m_pD3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds");
	
	// Create the light shader object.
	m_LightShader = new LightShader;
	// Initialize the light shader object.
	m_LightShader->Initialize(m_pD3D->GetDevice(), hwnd);

	// Create the light object.
	m_Light = new LightSystem;
	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);	

	/*
	// Create the texture shader object.
	// m_TextureShader = new TextureShader;
	// Initialize the texture shader object.
	// m_TextureShader->Initialize(m_pD3D->GetDevice(), hwnd);
	*/

	return true;
}

void GraphicSystem::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	/*
	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the color shader object.
	if(m_pColorShader)
	{
		m_pColorShader->Shutdown();
		delete m_pColorShader;
		m_pColorShader = 0;
	}
	*/
	// Release the model object.
	if(m_pModel)
	{
		m_pModel->Shutdown();
		delete m_pModel;
		m_pModel = 0;
	}

	// Release the camera object.
	if(m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = 0;
	}
	// Release the D3D object.
	if(m_pD3D)
	{
		m_pD3D->Shutdown();
		delete m_pD3D;
		m_pD3D = 0;
	}
	return;
}

bool GraphicSystem::Frame()
{
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.001f;
	
	if(rotation > 360.0f)
	{
		rotation = 0.0f;
	}
	
	// Render the graphics scene.
	Render(rotation);
	return true;
}

bool GraphicSystem::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pCamera->GetViewMatrix(viewMatrix);
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pModel->Render(m_pD3D->GetDeviceContext());

	// Render the model using the light shader.
	m_LightShader->Render(m_pD3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				       m_pModel->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}