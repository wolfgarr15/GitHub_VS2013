///////////////////////////////////
// Filename: cameraclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "cameraclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CameraClass::CameraClass(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& src) {}

CameraClass::~CameraClass() {}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void CameraClass::SetRotation(float rotX, float rotY, float rotZ)
{
	m_rotationX = rotX;
	m_rotationY = rotY;
	m_rotationZ = rotZ;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionY);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	float yaw;
	float pitch;
	float roll;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that defines the "up" direction.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//  Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup the default "look at" direction.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (rot about Y), pitch (rot about X), and roll (rot about Z).
	pitch = m_rotationX * (D3DX_PI / 180.0f);
	yaw = m_rotationY * (D3DX_PI / 180.0f);
	roll = m_rotationZ * (D3DX_PI / 180.0f);

	// Create the rotation matrix from the yaw, pitch, and roll.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the camera to the location of the viewer.
	lookAt = position + lookAt;

	// Create the updated view matrix.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::RenderReflection(float height)
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	float radians;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	position.x = m_positionX;
	position.y = -m_positionY + (height * 2.0f);
	position.z = m_positionZ;

	// Calculate the rotation in radians.
	radians = m_rotationY * (D3DX_PI / 180.0f);

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + m_positionX;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + m_positionZ;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_reflectionViewMatrix, &position, &lookAt, &up);

	return;
}

D3DXMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}
