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
	float m_CameraSpeed;
	float m_Pitch;
	float m_Yaw;
	float m_LastX;
	float m_LastY;
	bool m_FirstMouseMotion;
	float m_FOV;
	float m_Sensetivity;
	glm::vec3 m_Pos;
	glm::vec3 m_Up;
	glm::vec3 m_Camerafront;
	glm::vec3 m_Right;
	glm::vec3 m_Direction;
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
	void UpdateCameraVectors();
	glm::mat4 GetViewMatrix() const;
	//Updates the zoom 
	void Zoom();
	void ResetZoom();
};

#endif // !CAMERA_H
