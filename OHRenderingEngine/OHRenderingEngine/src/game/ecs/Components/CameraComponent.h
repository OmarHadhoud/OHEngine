#ifndef CAMERACOMP_H
#define CAMERACOMP_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"
#include "renderer/Camera.h"

//TODO: Turn it to a true component system instead of just wrapping the class in a pointer.
struct CameraCOMP : Component
{
	std::unique_ptr<Camera> m_Camera;
	bool m_Enabled;
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !CAMERACOMP_H

