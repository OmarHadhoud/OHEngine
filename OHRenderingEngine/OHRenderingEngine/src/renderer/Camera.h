#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/glm_util.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float NORMAL_SENSETIVITY = 0.2f;
const float NORMAL_CAM_SPEED = 5.5f;

enum MovementDirection
{
	kForward = 0,
	kBackward = 1,
	kRight = 2,
	kLeft = 3
};

class Camera
{
//Public variables
public:
	float m_camera_speed_;
	float m_pitch_;
	float m_yaw_;
	float m_last_x_;
	float m_last_y_;
	bool m_first_mouse_;
	float m_zoom_val_;
	float m_sensetivity_;
	glm::vec3 m_camera_pos_;
	glm::vec3 m_camera_up_;
	glm::vec3 m_camera_front_;
	glm::vec3 m_camera_right_;
	glm::vec3 m_direction_;
//Public functions
public:
	Camera(float sensetivity=NORMAL_SENSETIVITY, float speed=NORMAL_CAM_SPEED);
	~Camera();
	//Updates the camera each frame
	void Update(double xpos, double ypos);
	//Updates the camera rotation
	void UpdatePosition(MovementDirection dir, float delta_time);
	void UpdateSpeed(float speed);
	void ResetSpeed();
	glm::mat4 GetViewMatrix() const;
	//Updates the zoom 
	void Zoom();
	void ResetZoom();
};

#endif // !CAMERA_H
