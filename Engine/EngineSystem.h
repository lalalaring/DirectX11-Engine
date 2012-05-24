////////////////////////////////////////////////////////////////////////////////
// Filename: EngineSystem.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EngineSystem_H_
#define _EngineSystem_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InputSystem.h"
#include "GraphicSystem.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: EngineSystem
////////////////////////////////////////////////////////////////////////////////
class EngineSystem
{
public:
	EngineSystem();
	EngineSystem(const EngineSystem&);
	~EngineSystem();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputSystem* m_pInput;
	GraphicSystem* m_pGraphics;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/////////////
// GLOBALS //
/////////////
static EngineSystem* ApplicationHandle = 0;

#endif