////////////////////////////////////////////////////////////////////////////////
// Filename: CameraSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "CameraSystem.h"

CameraSystem::CameraSystem()
{
	m_fPhi		= 0.25f * MathHelper::Pi;
	m_fTheta	= 1.5f * m_fPhi;
	m_fRadius	= 5.0f;
}

CameraSystem::CameraSystem(const CameraSystem& other)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::SetPosition(float x, float y, float z)
{
	m_fPositionX = x;
	m_fPositionY = y;
	m_fPositionZ = z;
	return;
}

void CameraSystem::SetRotation(float x, float y, float z)
{
	m_fRotationX = x;
	m_fRotationY = y;
	m_fRotationZ = z;
	return;
}

XMVECTOR CameraSystem::GetPosition()
{
	return XMVectorSet(m_fPositionX, m_fPositionY, m_fPositionZ, 1.0f);
}

XMVECTOR CameraSystem::GetRotation()
{
	return XMVectorSet(m_fRotationX, m_fRotationY, m_fRotationZ, 1.0f);
}

void CameraSystem::Update()
{
	/*
	// Setup the position of the camera in the world.
	m_fPositionX = m_fRadius * sinf(m_fPhi)*cos(m_fTheta);
	m_fPositionZ = m_fRadius * sinf(m_fPhi)*sinf(m_fTheta);
	m_fPositionY = m_fRadius * cos(m_fTheta);
	*/

	// Setup the position of the camera in the world.
	m_fPositionX = 0.0f;
	m_fPositionZ = -10.0f;
	m_fPositionY = 0.0f;

	m_vPosition = XMVectorSet(m_fPositionX, m_fPositionY, m_fPositionZ, 1.0f);

	// Setup where the camera is looking by default.
	m_vLookAt = XMVectorZero();

	// Setup the vector that points upwards.
	m_vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Finally create the view matrix from the three updated vectors.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(m_vPosition, m_vLookAt, m_vUp);

	XMStoreFloat4x4(&m_mViewMatrix, viewMatrix);
	return;
}

void CameraSystem::GetViewMatrix(XMFLOAT4X4& viewMatrix)
{
	viewMatrix = m_mViewMatrix;
	return;
}
