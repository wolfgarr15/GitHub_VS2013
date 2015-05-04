/////////////////////////////////
// Filename: bitmapclass.h
/////////////////////////////////

/////////////////////////////////
// Pre-processing Directives
//------------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <D3D11.h>
#include <D3DX10math.h>
#include "textureclass.h"

///////////////////////////////
// Class Declaration
//-----------------------------
class BitmapClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass& src);
	~BitmapClass();

	bool Initialize(ID3D11Device* device,
					int screenWidth, int screenHeight,
					WCHAR* bitmapFilename,
					int bitmapWidth, int bitmapHeight);

	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* texFilename);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	TextureClass* m_texture;
	int m_screenWidth;
	int m_screenHeight;
	int m_bitmapWidth;
	int m_bitmapHeight;
	int m_previousPosX;
	int m_previousPosY;
};

