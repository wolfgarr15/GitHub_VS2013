////////////////////////////////
// Filename: modelclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include "textureclass.h"

using namespace std;

///////////////////////////////
// Class Declaration
//-----------------------------
class ModelClass
{

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x;
		float y;
		float z;

		float tu;
		float tv;

		float nx;
		float ny;
		float nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& src);
	~ModelClass();

	bool Initialize(ID3D11Device* device, char* modelFilename, WCHAR* texFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* texFilename);
	void ReleaseTexture();

	bool LoadModel(char* modelFilename);
	void ReleaseModel();

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	TextureClass* m_texture;
	ModelType* m_model;
};

