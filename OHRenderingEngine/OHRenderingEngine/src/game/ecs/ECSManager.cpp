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
int BoxCollider::m_Indices[MAX_ENTITY_COUNT];
int DirectionalLight::m_Indices[MAX_ENTITY_COUNT];
int LightShadow::m_Indices[MAX_ENTITY_COUNT];
int MeshRenderer::m_Indices[MAX_ENTITY_COUNT];
int PointLight::m_Indices[MAX_ENTITY_COUNT];
int RigidBody::m_Indices[MAX_ENTITY_COUNT];
int SpotLight::m_Indices[MAX_ENTITY_COUNT];
int Transform::m_Indices[MAX_ENTITY_COUNT];
////std::unordered_map<unsigned int, unsigned int> Camera::m_Map;
//std::unordered_map<int, int> DirectionalLight::m_Map;
//std::unordered_map<int, int> LightShadow::m_Map;
//std::unordered_map<int, int> MeshRenderer::m_Map;
//std::unordered_map<int, int> PointLight::m_Map;
//std::unordered_map<int, int> RigidBody::m_Map;
//std::unordered_map<int, int> SpotLight::m_Map;
//std::unordered_map<int, int> Transform::m_Map;
//



ECSManager::ECSManager()
{
}


ECSManager::~ECSManager()
{
}

void ECSManager::SetupComponents()
{
	for (int i = 0; i < MAX_ENTITY_COUNT; i++)
	{
		BoxCollider::m_Indices[i] = -1;
		DirectionalLight::m_Indices[i] = -1;
		LightShadow::m_Indices[i] = -1;
		MeshRenderer::m_Indices[i] = -1;
		PointLight::m_Indices[i] = -1;
		RigidBody::m_Indices[i] = -1;
		PointLight::m_Indices[i] = -1;
		SpotLight::m_Indices[i] = -1;
		Transform::m_Indices[i] = -1;
	}
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
	assert(BoxCollider::m_Indices[entityId] == -1);
	m_BoxColliders[BoxCollider::m_Count].m_EntityID = entityId;
	BoxCollider::m_Indices[entityId] = BoxCollider::m_Count;
	BoxCollider::m_Count++;
	return BoxCollider::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<DirectionalLight>(unsigned int entityId)
{
	assert(DirectionalLight::m_Count != MAX_DIRECTIONAL_LIGHTS_COUNT);
	assert(entityId < Entity::m_Count);
	assert(DirectionalLight::m_Indices[entityId] == -1);
	m_DirectionalLights[DirectionalLight::m_Count].m_EntityID = entityId;
	DirectionalLight::m_Indices[entityId] = DirectionalLight::m_Count;
	DirectionalLight::m_Count++;
	return DirectionalLight::m_Count - 1;
}


template<> unsigned int ECSManager::AddComponent<LightShadow>(unsigned int entityId)
{
	assert(LightShadow::m_Count != MAX_LIGHT_SHADHOWS_COUNT);
	assert(entityId < Entity::m_Count);
	assert(LightShadow::m_Indices[entityId] == -1);
	m_LightShadows[LightShadow::m_Count].m_EntityID = entityId;
	LightShadow::m_Indices[entityId] = LightShadow::m_Count;
	LightShadow::m_Count++;
	return LightShadow::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<MeshRenderer>(unsigned int entityId)
{
	assert(MeshRenderer::m_Count != MAX_MESH_RENDERER_COUNT);
	assert(entityId < Entity::m_Count);
	assert(MeshRenderer::m_Indices[entityId] == -1);
	m_MeshRenderers[MeshRenderer::m_Count].m_EntityID = entityId;
	MeshRenderer::m_Indices[entityId] = MeshRenderer::m_Count;
	MeshRenderer::m_Count++;
	return MeshRenderer::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<PointLight>(unsigned int entityId)
{
	assert(PointLight::m_Count != MAX_POINT_LIGHT_COUNT);
	assert(entityId < Entity::m_Count);
	assert(PointLight::m_Indices[entityId] == -1);
	m_PointLights[PointLight::m_Count].m_EntityID = entityId;
	PointLight::m_Indices[entityId] = PointLight::m_Count;
	PointLight::m_Count++;
	return PointLight::m_Count - 1;
}


template<> unsigned int ECSManager::AddComponent<RigidBody>(unsigned int entityId)
{
	assert(RigidBody::m_Count != MAX_RIGID_BODY_COUNT);
	assert(entityId < Entity::m_Count);
	assert(RigidBody::m_Indices[entityId] == -1);
	m_RigidBodies[RigidBody::m_Count].m_EntityID = entityId;
	RigidBody::m_Indices[entityId] = RigidBody::m_Count;
	RigidBody::m_Count++;
	return RigidBody::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<SpotLight>(unsigned int entityId)
{
	assert(SpotLight::m_Count != MAX_SPOTLIGHT_COUNT);
	assert(entityId < Entity::m_Count);
	assert(SpotLight::m_Indices[entityId] == -1);
	m_SpotLights[SpotLight::m_Count].m_EntityID = entityId;
	SpotLight::m_Indices[entityId] = SpotLight::m_Count;
	SpotLight::m_Count++;
	return SpotLight::m_Count - 1;
}

template<> unsigned int ECSManager::AddComponent<Transform>(unsigned int entityId)
{
	assert(Transform::m_Count != MAX_TRANSFORM_COUNT);
	assert(entityId < Entity::m_Count);
	assert(Transform::m_Indices[entityId] == -1);
	m_Transforms[Transform::m_Count].m_EntityID = entityId;
	Transform::m_Indices[entityId] = Transform::m_Count;
	Transform::m_Count++;
	return Transform::m_Count - 1;
}