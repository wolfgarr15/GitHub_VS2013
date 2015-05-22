///////////////////////////////////
// Filename: positionclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "positionclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
PositionClass::PositionClass() 
{
	m_frameTime = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
}

PositionClass::PositionClass(const PositionClass& src) {}

PositionClass::~PositionClass() {}

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void PositionClass::GetRotation(float& rotationX, float& rotationY)
{
	rotationX = m_rotationX;
	rotationY = m_rotationY;
	return;
}

void PositionClass::TurnUp(bool keyDown)
{
	// If the key is pressed increase the speed at which the camera turns up.
	if (keyDown)
	{
		m_upTurnSpeed += m_frameTime * 0.01f;
		// Sets the maximum turning speed.
		if (m_upTurnSpeed > (m_frameTime * 0.15f))
			m_upTurnSpeed = m_frameTime * 0.15f;
	}
	else
	{
		m_upTurnSpeed -= m_frameTime * 0.005f;
		// Prevent the speed from being negative.
		if (m_upTurnSpeed < 0.0f)
			m_upTurnSpeed = 0.0f;
	}

	// Update the rotation rate using the trun speed.
	m_rotationX -= m_upTurnSpeed;
	if (m_rotationY < 0)
		m_rotationX += 360.0f;

	return;
}

void PositionClass::TurnDown(bool keyDown)
{
	// If the key is pressed increase the speed at which the camera turns down.
	if (keyDown)
	{
		m_downTurnSpeed += m_frameTime * 0.01f;
		if (m_downTurnSpeed > (m_frameTime * 0.15f))
			m_downTurnSpeed = m_frameTime * 0.15f;
	}
	else
	{
		m_downTurnSpeed -= m_frameTime* 0.005f;
		if (m_downTurnSpeed < 0.0f)
			m_downTurnSpeed = 0.0f;
	}

	// Update the rotation using the turning speed.
	m_rotationX += m_downTurnSpeed;
	if (m_rotationX > 360.0f)
		m_rotationX -= 360.0f;

	return;
}

void PositionClass::TurnLeft(bool keyDown)
{
	// If the key is pressed increase the speed at which the camera turns left.
	if (keyDown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;
		// Sets the maximum turning speed.
		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
			m_leftTurnSpeed = m_frameTime * 0.15f;
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.005f;
		// Prevent the speed from being negative.
		if (m_leftTurnSpeed < 0.0f)
			m_leftTurnSpeed = 0.0f;
	}

	// Update the rotation rate using the trun speed.
	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0)
		m_rotationY += 360.0f;

	return;
}

void PositionClass::TurnRight(bool keyDown)
{
	// If the key is pressed increase the speed at which the camera turns right.
	if (keyDown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;
		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
			m_rightTurnSpeed = m_frameTime * 0.15f;
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;
		if (m_rightTurnSpeed < 0.0f)
			m_rightTurnSpeed = 0.0f;
	}

	// Update the rotation using the turning speed.
	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY > 360.0f)
		m_rotationY -= 360.0f;

	return;
}
