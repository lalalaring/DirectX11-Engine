////////////////////////////////////////////////////////////////////////////////
// Filename: CameraSystem.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CameraSystem_H_
#define _CameraSystem_H_


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <xnamath.h>
#include <MathHelper.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraSystem
////////////////////////////////////////////////////////////////////////////////
class CameraSystem
{
public:
	CameraSystem();
	CameraSystem(const CameraSystem&);
	~CameraSystem();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMVECTOR GetPosition();
	XMVECTOR GetRotation();

	void Update();
	void GetViewMatrix(XMFLOAT4X4&);

private:
	float		m_fPositionX, m_fPositionY, m_fPositionZ;
	float		m_fRotationX, m_fRotationY, m_fRotationZ;

	XMFLOAT4X4	m_mViewMatrix;
	XMVECTOR	m_vUp, m_vPosition, m_vLookAt;

	float		m_fTheta;
	float		m_fPhi;
	float		m_fRadius;

};

#endif