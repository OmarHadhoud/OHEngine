#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"

//TODO: Turn it to a true component system instead of just wrapping the class in a pointer.
struct Camera : Component
{
	float m_Sensetivity;
	float m_Pitch;
	float m_Yaw;
	float m_FOV;
	float m_NearPlane;
	float m_FarPlane;
	bool m_YLocked;
	bool m_Enabled;
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !CAMERA_H

