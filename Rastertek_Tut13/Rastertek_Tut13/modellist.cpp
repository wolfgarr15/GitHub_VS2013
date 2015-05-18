///////////////////////////////////
// Filename: modellist.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "modellist.h"

///////////////////////////////
// Class Definition
//-----------------------------
ModelList::ModelList()
{
	m_ModelInfoList = 0;
}

ModelList::ModelList(const ModelList& src) {}

ModelList::~ModelList() {}

bool ModelList::Initialize(int modelCount)
{
	float red;
	float green;
	float blue;

	// Store the model count.
	m_modelCount = modelCount;

	// Create the model list array.
	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if (!m_ModelInfoList)
		return false;

	//  Seed the random number generator with the current time.
	srand((UINT)time(NULL));

	// Generate a random color and position for each model.
	for (int i = 0; i < m_modelCount; i++)
	{
		// Generate the random color.
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		m_ModelInfoList[i].color = D3DXVECTOR4(red, green, blue, 1.0f);

		// Generate the random position.
		m_ModelInfoList[i].positionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].positionZ = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
	}

	return true;
}

void ModelList::Shutdown()
{
	// Release the model information list.
	if (m_ModelInfoList)
	{
		delete [] m_ModelInfoList;
		m_ModelInfoList = 0;
	}

	return;
}

int ModelList::GetModelCount()
{
	return m_modelCount;
}

void ModelList::GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color)
{
	positionX = m_ModelInfoList[index].positionX;
	positionY = m_ModelInfoList[index].positionY;
	positionZ = m_ModelInfoList[index].positionZ;

	color = m_ModelInfoList[index].color;

	return;
}
