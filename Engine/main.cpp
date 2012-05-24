////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	EngineSystem* m_pSystem;
	
	// Create the system object.
	m_pSystem = new EngineSystem;

	// Initialize and run the system object.
	m_pSystem->Initialize();
	m_pSystem->Run();

	// Shutdown and release the system object.
	m_pSystem->Shutdown();
	delete m_pSystem;
	m_pSystem = 0;

	return 0;
}