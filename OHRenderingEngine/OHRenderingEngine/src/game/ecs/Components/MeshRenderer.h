#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <unordered_map>
#include <glm/glm.hpp>

#include "renderer/Model.h"

struct MeshRenderer
{
	std::unique_ptr<Model> m_Model;
	glm::vec3 m_BorderColor;
	bool m_BorderEnabled;
	bool m_IsTransparent;
	bool m_IsSolid;
	bool m_Enabled;
	static unsigned int m_Count;
	static std::unordered_map<unsigned int, unsigned int> m_Map;
};

#endif // !MESH_RENDERER_H

