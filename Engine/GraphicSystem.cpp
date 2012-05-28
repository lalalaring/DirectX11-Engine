////////////////////////////////////////////////////////////////////////////////
// Filename: GraphicSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "GraphicSystem.h"

GraphicSystem::GraphicSystem()
{
	m_pD3D			= 0;
	m_pCamera		= 0;
	m_pModel		= 0;
	m_pColorShader	= 0;
	m_fPhi			= MathHelper::Pi;
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
	m_pModel->Initialize(m_pD3D->GetDevice());

	// Create the color shader object.
	m_pColorShader = new ColorShader;

	// Initialize the color shader object.
	m_pColorShader->Initialize(m_pD3D->GetDevice(), hwnd);

	return true;
}

void GraphicSystem::Shutdown()
{
	// Release the color shader object.
	if(m_pColorShader)
	{
		m_pColorShader->Shutdown();
		delete m_pColorShader;
		m_pColorShader = 0;
	}
	
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
	rotation += m_fPhi * 0.001f;
	
	if(rotation > 360.0f)
	{
		rotation = 0.0f;
	}
	
	// Render the graphics scene.
	Update(rotation);
	return true;
}

bool GraphicSystem::Update(float rotation)
{
	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pCamera->Update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pCamera->GetViewMatrix(m_mViewMatrix);
	m_pD3D->GetWorldMatrix(m_mWorldMatrix);
	m_pD3D->GetProjectionMatrix(m_mProjectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pModel->Update(m_pD3D->GetDeviceContext());
	
	// Render the model using the color shader.
	m_pColorShader->Update(m_pD3D->GetDeviceContext(), m_pModel->GetIndexCount(), m_mWorldMatrix, m_mViewMatrix, m_mProjectionMatrix);

	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}