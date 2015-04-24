/////////////////////////////////
// Filename: textureclass.h
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
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& src);
	~TextureClass();

	bool Initialize(ID3D11Device* device, WCHAR* texFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

