#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <glm/glm.hpp>

#include "game/ecs/Components/Component.h"
#include "renderer/Model.h"

enum class Transparency
{
	kNonTransparent,
	kSemiTransparent,
	kTransparent
};

struct MeshRenderer : Component
{
	std::unique_ptr<Model> m_Model;
	glm::vec3 m_BorderColor;
	bool m_BorderEnabled;
	Transparency m_Transparency;
	bool m_IsSolid;
	static unsigned int m_Count;
 static std::unordered_map<int, int> m_Map;
};

#endif // !MESH_RENDERER_H

