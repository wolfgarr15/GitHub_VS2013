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
		float rotationX;
		float rotationY;
		float rotationZ;
	};

public:
	ModelList();
	ModelList(const ModelList& src);
	~ModelList();

	bool Initialize(int modelCount);
	void Shutdown();

	int GetModelCount();
	void GetData(int index, D3DXVECTOR4& color, 
				float& positionX, float& positionY, float& positionZ, 
				float& rotationX, float& rotationY, float& rotationZ);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

