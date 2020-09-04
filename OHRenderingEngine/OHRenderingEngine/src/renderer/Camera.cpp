#include "Camera.h"

Camera::Camera(float sensetivity, float speed):
	m_sensetivity_(sensetivity),
	m_camera_speed_(speed),
	m_camera_pos_(glm::vec3(0.0f,0.0f,10.0f)),
	m_camera_up_(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_camera_front_(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_camera_right_(glm::vec3(1.0f,0.0f,0.0f)),
	m_zoom_val_(45.0f),
	m_first_mouse_(true),
	m_yaw_(-90),
	m_pitch_(0)
{

}


Camera::~Camera()
{
}

void Camera::Update(double xpos, double ypos)
{
	//If first time to hold the cursor in the window
	if (m_first_mouse_)
	{
		m_last_x_ = xpos;
		m_last_y_ = ypos;
		m_first_mouse_ = false;
	}
	//Get the offset of the mouse from last position to move the camera
	float xOffset = xpos - m_last_x_;
	float yOffset = m_last_y_ - ypos;
	m_last_x_ = xpos;
	m_last_y_ = ypos;

	//Update using the mouse sensitivity
	xOffset *= m_sensetivity_;
	yOffset *= m_sensetivity_;
	
	//Update the rotation angles
	m_yaw_ += xOffset;
	m_pitch_ += yOffset;
	m_pitch_ = m_pitch_ > 89.0f ? 89.0f : m_pitch_;
	m_pitch_ = m_pitch_ < -89.0f ? -89.0f : m_pitch_;
	m_direction_.y = sin(glm::radians(m_pitch_));
	m_direction_.x = cos(glm::radians(m_pitch_)) * cos(glm::radians(m_yaw_));
	m_direction_.z = cos(glm::radians(m_pitch_)) * sin(glm::radians(m_yaw_));
	
	m_camera_front_ = glm::normalize(m_direction_);
}

void Camera::UpdatePosition(MovementDirection dir, float delta_time)
{
	m_camera_right_ = glm::normalize(glm::cross(m_camera_front_, m_camera_up_));
	m_camera_up_ = glm::normalize(glm::cross(m_camera_right_, m_camera_front_));
	switch (dir)
	{
	case kForward: 
		m_camera_pos_ += m_camera_speed_ * delta_time * m_camera_front_;
		break;
	case kBackward: 
		m_camera_pos_ -= m_camera_speed_ * delta_time * m_camera_front_;
		break;
	case kRight: 
		m_camera_pos_ += m_camera_right_ * delta_time * m_camera_speed_;
		break;
	case kLeft: 
		m_camera_pos_ -= m_camera_right_ * delta_time * m_camera_speed_;
		break;
	}
}

void Camera::UpdateSpeed(float speed)
{
	m_camera_speed_ = speed > 0 ? speed : m_camera_speed_;
}

void Camera::ResetSpeed()
{
	m_camera_speed_ = NORMAL_CAM_SPEED;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_camera_pos_, m_camera_pos_ + m_camera_front_, m_camera_up_);
}


void Camera::Zoom()
{
	this->m_zoom_val_--;
	if (this->m_zoom_val_ <= 20.0f)
		this->m_zoom_val_ = 20.0f;
}

void Camera::ResetZoom()
{
	this->m_zoom_val_ = 45.0f;
}

