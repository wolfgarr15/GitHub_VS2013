////////////////////////////////
// Filename: inputclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once

///////////////////////////////
// Class Declaration
//-----------------------------
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass& src);
	~InputClass();

	bool Initialize();

	void KeyDown(UINT key);
	void KeyUp(UINT key);

	bool IsKeyDown(UINT key);
	bool IsKeyUp(UINT key);

private:
	bool m_Keys[256];
};

