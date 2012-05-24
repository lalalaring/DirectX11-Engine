////////////////////////////////////////////////////////////////////////////////
// Filename: InputSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "InputSystem.h"

InputSystem::InputSystem()
{
}

InputSystem::InputSystem(const InputSystem& other)
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::Initialize()
{
	int i;
	
	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_bKeys[i] = false;
	}
	return;
}

void InputSystem::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_bKeys[input] = true;
	return;
}

void InputSystem::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_bKeys[input] = false;
	return;
}

bool InputSystem::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_bKeys[key];
}