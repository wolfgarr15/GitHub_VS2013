/////////////////////////////////
// Filename: texturearrayclass.h
/////////////////////////////////

/////////////////////////////////
// Pre-processing Directives
//------------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <D3D11.h>
#include <D3DX11tex.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass& src);
	~TextureArrayClass();

	bool Initialize(ID3D11Device* device, WCHAR* texFilename1, WCHAR* texFilename2);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[2];
};

