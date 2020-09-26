#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <glm/glm.hpp>

#include "renderer/Model.h"

struct MeshRenderer
{
	std::auto_ptr<Model> m_Model;
	glm::vec3 m_BorderColor;
	bool m_BorderEnabled;
	bool m_IsTransparent;
	bool m_IsSolid;
	bool m_Enabled;
};

#endif // !MESH_RENDERER_H

