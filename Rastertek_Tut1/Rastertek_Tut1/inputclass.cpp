////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "inputclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
InputClass::InputClass() {}

InputClass::InputClass(const InputClass& src) {}

InputClass::~InputClass() {}

void InputClass::Initialize()
{
	// Initialize the key array.
	for(int i = 0; i < 256; i++)
		m_Keys[i] = false;

	return;
}

void InputClass::KeyDown(UINT input)
{
	m_Keys[input] = true;
	return;
}

void InputClass::KeyUp(UINT input)
{
	m_Keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(UINT key)
{
	return m_Keys[key];
}