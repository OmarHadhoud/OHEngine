#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/glm_util.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/enums.h"

const float NORMAL_SENSETIVITY = 0.2f;
const float NORMAL_CAM_SPEED = 12.0f;


class Camera
{
public:
	Camera(float sensetivity=NORMAL_SENSETIVITY, float speed=NORMAL_CAM_SPEED);
	~Camera();

	//Updates the camera after moving mouse
	//xpos: The mouse x coordinate
	//ypos: The mouse y coordinate.
	void UpdateRotation(double xpos, double ypos);
	
	//Updates the camera after moving it
	//dir: Enum for direction of movement, forward/backward/right/left
	//delta_time: Time between frames
	void UpdatePosition(MovementDirection dir, float delta_time);
	
	//Resets the camera walking speed to the default one
	void ResetSpeed();
	
	//Zooms using the camera 
	void Zoom();

	//Resets the zoom value for the camera
	void ResetZoom();
	
	//Getters
	float GetSpeed() const;
	float GetFOV() const;
	float GetSensetivity() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetFront() const;
	glm::mat4 GetViewMatrix() const;
	
	//Setters
	void SetCameraSpeed(float value);

private:
	float m_CameraSpeed;
	float m_Pitch;
	float m_Yaw;
	float m_LastX;
	float m_LastY;
	bool m_FirstMouseMotion;
	float m_FOV;
	float m_Sensetivity;
	bool m_YLocked;
	glm::vec3 m_Pos;
	glm::vec3 m_Up;
	glm::vec3 m_Camerafront;
	glm::vec3 m_Right;
	glm::vec3 m_Direction;

	//Updates all the vectors that are used within the camera.
	void UpdateCameraVectors();
};

#endif // !CAMERA_H
