#ifndef ECSMANAGER_H
#define ECSMANAGER_H

#include "game/ecs/Components/BoxCollider.h"
#include "game/ecs/Components/CameraComponent.h"
#include "game/ecs/Components/DirectionalLight.h"
#include "game/ecs/Components/LightShadow.h"
#include "game/ecs/Components/MeshRenderer.h"
#include "game/ecs/Components/PointLight.h"
#include "game/ecs/Components/RigidBody.h"
#include "game/ecs/Components/SpotLight.h"
#include "game/ecs/Components/Transform.h"

#include "game/ecs/Entity.h"
#include "game/ecs/ECSCount.h"


class ECSManager
{
public:
	ECSManager();
	~ECSManager();
	void SetupComponents();
	//Entities
	Entity m_Entities[MAX_ENTITY_COUNT];
	//Components
	BoxCollider m_BoxColliders[MAX_BOX_COLLIDERS_COUNT];
	CameraCOMP m_Cameras[MAX_CAMERA_COUNT];
	DirectionalLight m_DirectionalLights[MAX_DIRECTIONAL_LIGHTS_COUNT];
	LightShadow m_LightShadows[MAX_LIGHT_SHADHOWS_COUNT];
	MeshRenderer m_MeshRenderers[MAX_MESH_RENDERER_COUNT];
	PointLight m_PointLights[MAX_POINT_LIGHT_COUNT];
	RigidBody m_RigidBodies[MAX_RIGID_BODY_COUNT];
	SpotLight m_SpotLights[MAX_SPOTLIGHT_COUNT];
	Transform m_Transforms[MAX_TRANSFORM_COUNT];
	//Add Entity/Components functions
	int AddEntities(int count);
	template<typename T> unsigned int AddComponent(unsigned int entityId);
};


#endif // !ECSMANAGER_H

