////////////////////////////////////////////////////////////////////////////////
// Filename: LightSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "LightSystem.h"

LightSystem::LightSystem()
{
}

LightSystem::LightSystem(const LightSystem& other)
{
}

LightSystem::~LightSystem()
{
}

void LightSystem::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightSystem::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

D3DXVECTOR4 LightSystem::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 LightSystem::GetDirection()
{
	return m_direction;
}

