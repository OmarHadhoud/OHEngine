#include "ECSManager.h"
#include <cassert>

//STATIC VARIABLES
//COMPONENTS COUNT
unsigned int Entity::m_Count = 0;
unsigned int BoxCollider::m_Count = 0;
//unsigned int Camera::m_Count = 0;
unsigned int DirectionalLight::m_Count = 0;
unsigned int LightShadow::m_Count = 0;
unsigned int MeshRenderer::m_Count = 0;
unsigned int PointLight::m_Count = 0;
unsigned int RigidBody::m_Count = 0;
unsigned int SpotLight::m_Count = 0;
unsigned int Transform::m_Count = 0;
//MAPS
std::unordered_map<int, int> BoxCollider::m_Map;
//std::unordered_map<unsigned int, unsigned int> Camera::m_Map;
std::unordered_map<int, int> DirectionalLight::m_Map;
std::unordered_map<int, int> LightShadow::m_Map;
std::unordered_map<int, int> MeshRenderer::m_Map;
std::unordered_map<int, int> PointLight::m_Map;
std::unordered_map<int, int> RigidBody::m_Map;
std::unordered_map<int, int> SpotLight::m_Map;
std::unordered_map<int, int> Transform::m_Map;




ECSManager::ECSManager()
{
}


ECSManager::~ECSManager()
{
}

int ECSManager::AddEntities(int count)
{
	int entitiesCount = Entity::m_Count;
	assert(entitiesCount + count <= MAX_ENTITY_COUNT);

	for (int i = 0; i < count; i++)
	{
		m_Entities[entitiesCount].m_Id = entitiesCount+i;
	}
	Entity::m_Count = entitiesCount + count;
	return entitiesCount;
}


template<typename T> unsigned int ECSManager::AddComponent(unsigned int entityId)
{
	static_assert(false);
	return -1;
}

template<> unsigned int ECSManager::AddComponent<BoxCollider>(unsigned int entityId)
{
	assert(BoxCollider::m_Count != MAX_BOX_COLLIDERS_COUNT);
	assert(entityId < Entity::m_Count);
	assert(BoxCollider::m_Map.find(entityId) == BoxCollider::m_Map.end());
	BoxCollider::m_Map[entityId] = BoxCollider::m_Count;
	BoxCollider::m_Count++;
	return BoxCollider::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<DirectionalLight>(unsigned int entityId)
{
	assert(DirectionalLight::m_Count != MAX_DIRECTIONAL_LIGHTS_COUNT);
	assert(entityId < Entity::m_Count);
	assert(DirectionalLight::m_Map.find(entityId) == DirectionalLight::m_Map.end());
	DirectionalLight::m_Map[entityId] = DirectionalLight::m_Count;
	DirectionalLight::m_Count++;
	return DirectionalLight::m_Count - 1;
}


template<> unsigned int ECSManager::AddComponent<LightShadow>(unsigned int entityId)
{
	assert(LightShadow::m_Count != MAX_LIGHT_SHADHOWS_COUNT);
	assert(entityId < Entity::m_Count);
	assert(LightShadow::m_Map.find(entityId) == LightShadow::m_Map.end());
	LightShadow::m_Map[entityId] = LightShadow::m_Count;
	LightShadow::m_Count++;
	return LightShadow::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<MeshRenderer>(unsigned int entityId)
{
	assert(MeshRenderer::m_Count != MAX_MESH_RENDERER_COUNT);
	assert(entityId < Entity::m_Count);
	assert(MeshRenderer::m_Map.find(entityId) == MeshRenderer::m_Map.end());
	MeshRenderer::m_Map[entityId] = MeshRenderer::m_Count;
	MeshRenderer::m_Count++;
	return MeshRenderer::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<PointLight>(unsigned int entityId)
{
	assert(PointLight::m_Count != MAX_POINT_LIGHT_COUNT);
	assert(entityId < Entity::m_Count);
	assert(PointLight::m_Map.find(entityId) == PointLight::m_Map.end());
	PointLight::m_Map[entityId] = PointLight::m_Count;
	PointLight::m_Count++;
	return PointLight::m_Count - 1;
}


template<> unsigned int ECSManager::AddComponent<RigidBody>(unsigned int entityId)
{
	assert(RigidBody::m_Count != MAX_RIGID_BODY_COUNT);
	assert(entityId < Entity::m_Count);
	assert(RigidBody::m_Map.find(entityId) == RigidBody::m_Map.end());
	RigidBody::m_Map[entityId] = RigidBody::m_Count;
	RigidBody::m_Count++;
	return RigidBody::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<SpotLight>(unsigned int entityId)
{
	assert(SpotLight::m_Count != MAX_SPOTLIGHT_COUNT);
	assert(entityId < Entity::m_Count);
	assert(SpotLight::m_Map.find(entityId) == SpotLight::m_Map.end());
	SpotLight::m_Map[entityId] = SpotLight::m_Count;
	SpotLight::m_Count++;
	return SpotLight::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<Transform>(unsigned int entityId)
{
	assert(Transform::m_Count != MAX_TRANSFORM_COUNT);
	assert(entityId < Entity::m_Count);
	assert(Transform::m_Map.find(entityId) == Transform::m_Map.end());
	Transform::m_Map[entityId] = Transform::m_Count;
	Transform::m_Count++;
	return Transform::m_Count - 1;
}