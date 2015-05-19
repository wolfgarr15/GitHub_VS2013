///////////////////////////////////
// Filename: textclass.h
///////////////////////////////////

///////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include "fontclass.h"
#include "fontshaderclass.h"

///////////////////////////////
// Class Declaration
//-----------------------------
class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		int vertexCount;
		int indexCount;
		int maxLength;
		float red;
		float green;
		float blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass& src);
	~TextClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd,
					int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix,
					int numSentences);

	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthomatrix);

	bool SetFPS(int FPScount, ID3D11DeviceContext* deviceContext);
	bool SetCPU(int CPUusage, ID3D11DeviceContext* deviceContext);
	bool SetRenderCount(int renderCount, ID3D11DeviceContext* deviceContext);

private:
	bool InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);

	bool UpdateSentence(SentenceType* sentence, char* text,
						int positionX, int positionY,
						float red, float green, float blue,
						ID3D11DeviceContext* deviceContext);

	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

private:
	FontClass* m_font;
	FontShaderClass* m_fontShader;
	int m_screenWidth;
	int m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;
	SentenceType** m_sentences;
	int m_numSentences;
};

