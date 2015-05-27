///////////////////////////////////
// Filename: texturearrayclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "texturearrayclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = 0;
	m_textures[1] = 0;
	m_textures[2] = 0;
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass& src) {}

TextureArrayClass::~TextureArrayClass() {}

bool TextureArrayClass::Initialize(ID3D11Device* device, WCHAR* texFilename1, WCHAR* texFilename2, WCHAR* alphaFilename)
{
	HRESULT result;

	// Create textures from the resource files.
	result = D3DX11CreateShaderResourceViewFromFileW(device, texFilename1, NULL, NULL, &m_textures[0], NULL);
	if (FAILED(result))
		return false;

	result = D3DX11CreateShaderResourceViewFromFileW(device, texFilename2, NULL, NULL, &m_textures[1], NULL);
	if (FAILED(result))
		return false;

	result = D3DX11CreateShaderResourceViewFromFileW(device, alphaFilename, NULL, NULL, &m_textures[2], NULL);
	if (FAILED(result))
		return false;

	return true;
}

void TextureArrayClass::Shutdown()
{
	// Release each element of the texture array.
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	if (m_textures[2])
	{
		m_textures[2]->Release();
		m_textures[2] = 0;
	}

	return;
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}
