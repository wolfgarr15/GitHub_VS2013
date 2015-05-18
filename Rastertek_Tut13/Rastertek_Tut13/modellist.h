/////////////////////////////////
// Filename: modellist.h
/////////////////////////////////

/////////////////////////////////
// Pre-processing Directives
//------------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <D3DX10math.h>
#include <stdlib.h>
#include <time.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class ModelList
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float positionX;
		float positionY;
		float positionZ;
	};

public:
	ModelList();
	ModelList(const ModelList& src);
	~ModelList();

	bool Initialize(int modelCount);
	void Shutdown();

	int GetModelCount();
	void GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

