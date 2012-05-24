////////////////////////////////////////////////////////////////////////////////
// Filename: GraphicSystem.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GraphicSystem_H_
#define _GraphicSystem_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dSystem.h"
#include "CameraSystem.h"
#include "ModelSystem.h"
// #include "ColorShader.h"
// #include "TextureShader.h"
#include "LightShader.h"
#include "LightSystem.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicSystem
////////////////////////////////////////////////////////////////////////////////
class GraphicSystem
{
public:

	GraphicSystem();
	GraphicSystem(const GraphicSystem&);
	~GraphicSystem();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:

	bool Render(float);

private:

	d3dSystem* m_pD3D;
	CameraSystem* m_pCamera;
	ModelSystem* m_pModel;
	// ColorShader* m_pColorShader;
	// TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	LightSystem* m_Light;
};

#endif