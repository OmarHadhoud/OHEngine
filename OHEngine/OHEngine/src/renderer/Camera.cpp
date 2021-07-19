#include "Camera.h"

CameraDeleted::CameraDeleted(float sensetivity, float speed):
	m_CameraSpeed(speed),
	m_Yaw(-90),
	m_Pitch(0),
	m_LastX(0),
	m_LastY(0),
	m_FirstMouseMotion(true),
	m_FOV(45.0f),
	m_Sensetivity(sensetivity),
	m_YLocked(true),
	m_Pos(glm::vec3(0.0f,1.0f,20.0f)),
	m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_Camerafront(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_Right(glm::vec3(1.0f,0.0f,0.0f)),
	m_Direction(glm::vec3(0.0f))
{
}


CameraDeleted::~CameraDeleted()
{
}

void CameraDeleted::UpdateRotation(double xpos, double ypos)
{
	//If first time to hold the cursor in the window
	if (m_FirstMouseMotion)
	{
		m_LastX = xpos;
		m_LastY = ypos;
		m_FirstMouseMotion = false;
	}
	//Get the offset of the mouse from last position to move the camera
	float xOffset = xpos - m_LastX;
	float yOffset = m_LastY - ypos;
	m_LastX = xpos;
	m_LastY = ypos;

	//Update using the mouse sensitivity
	xOffset *= m_Sensetivity;
	yOffset *= m_Sensetivity;
	
	//Update the rotation angles
	m_Yaw += xOffset;
	m_Pitch += yOffset;

	m_Yaw = m_Yaw > 360.0f ? m_Yaw - 360.f : m_Yaw;
	m_Yaw = m_Yaw < 0.0f ? m_Yaw + 360.f : m_Yaw;
	m_Pitch = m_Pitch > 89.0f ? 89.0f : m_Pitch;
	m_Pitch = m_Pitch < -89.0f ? -89.0f : m_Pitch;
	
	UpdateCameraVectors();
}

void CameraDeleted::UpdatePosition(MovementDirection dir, float delta_time)
{
	glm::vec3 movement_direction = m_Camerafront;
	if (m_YLocked) movement_direction.y = 0;
	switch (dir)
	{
	case MovementDirection::kForward: 
		m_Pos += movement_direction * m_CameraSpeed * delta_time;
		break;
	case MovementDirection::kBackward:
		m_Pos -= movement_direction * m_CameraSpeed * delta_time;
		break;
	case MovementDirection::kRight:
		m_Pos += m_Right * delta_time * m_CameraSpeed;
		break;
	case MovementDirection::kLeft:
		m_Pos -= m_Right * delta_time * m_CameraSpeed;
		break;
	}
}

void CameraDeleted::ResetSpeed()
{
	m_CameraSpeed = NORMAL_CAM_SPEED;
}

void CameraDeleted::UpdateCameraVectors()
{
	m_Direction.y = sin(glm::radians(m_Pitch));
	m_Direction.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	m_Direction.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));

	m_Camerafront = glm::normalize(m_Direction);
	m_Right = glm::normalize(glm::cross(m_Camerafront, glm::vec3(0,1,0)));
	m_Up = glm::normalize(glm::cross(m_Right, m_Camerafront));
}

glm::mat4 CameraDeleted::GetViewMatrix() const
{
	return glm::lookAt(m_Pos, m_Pos + m_Camerafront, m_Up);
}

void CameraDeleted::SetCameraSpeed(float value)
{
	m_CameraSpeed = value > 0 ? value : m_CameraSpeed;
}


void CameraDeleted::Zoom()
{
	this->m_FOV--;
	if (this->m_FOV <= 20.0f)
		this->m_FOV = 20.0f;
}

void CameraDeleted::ResetZoom()
{
	this->m_FOV = 45.0f;
}

float CameraDeleted::GetSpeed() const
{
	return m_CameraSpeed;
}

float CameraDeleted::GetFOV() const
{
	return m_FOV;
}

float CameraDeleted::GetSensetivity() const
{
	return m_Sensetivity;
}

glm::vec3 CameraDeleted::GetPosition() const
{
	return m_Pos;
}

glm::vec3 CameraDeleted::GetFront() const
{
	return m_Camerafront;
}

