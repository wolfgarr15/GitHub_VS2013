///////////////////////////////////
// Filename: textureclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "textureclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::TextureClass(const TextureClass& src) {}

TextureClass::~TextureClass() {}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* texFilename)
{
	HRESULT result;

	// Create the texture from the resource file.
	result = D3DX11CreateShaderResourceViewFromFileW(device, texFilename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
		return false;

	return true;
}

void TextureClass::Shutdown()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}
