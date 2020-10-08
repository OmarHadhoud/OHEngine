#include "GameLogicSystem.h"
#include <iostream>
#include <glm/glm.hpp>
#include "game/Game.h"

GameLogicSystem::GameLogicSystem()
{
}


GameLogicSystem::~GameLogicSystem()
{
}



void GameLogicSystem::Update()
{
	ProcessEvents(); 
	UpdateRigidBodyComponents();
	DisableDeadEntities();
}

GameState GameLogicSystem::GetGameState() const
{
	return m_gameState;
}

void GameLogicSystem::ProcessEvent(Event* event)
{
	switch (event->m_EventType)
	{
	case EventType::kMovePlayer:
	{
		MovePlayerEvent* moveEvent = dynamic_cast<MovePlayerEvent*>(event);
		m_ECSManager->m_RigidBodies[0].m_Acceleration = glm::vec3(1.0f);
		UpdateCameraPosition(0, moveEvent->m_MovementDirection, Game::m_DeltaTime);
		break;
	}
	case EventType::kRotatePlayer:
	{
		RotatePlayerEvent* rotateEvent = dynamic_cast<RotatePlayerEvent*> (event);
		UpdateCameraRotation(0, rotateEvent->m_MouseXPos, rotateEvent->m_MouseYPos, rotateEvent->m_LastMouseXPos, rotateEvent->m_LastMouseYPos);
		break;
	}
	case EventType::kExitGame:
	{
		m_gameState = GameState::kGameExited;
		break;
	}
	case EventType::kPlayerShoots:
	{
		PlayerMouseClickEvent* pressEvent = dynamic_cast<PlayerMouseClickEvent*> (event);
		glm::vec3 rayNormalized = GetRayCameraNormalized(pressEvent->m_MouseXPos, pressEvent->m_MouseYPos);
		int hitEntity = GetRayPickedEntityID(rayNormalized);
		ShootEntity(hitEntity);
		break;
	}
	case EventType::kPlayerSelects:
	{
		break;
	}
	default:
		break;
	}
}

void GameLogicSystem::UpdateCameraPosition(int index, MovementDirection dir, float delta_time)
{
	int transformIndex = Transform::m_Indices[m_ECSManager->m_Cameras[0].m_EntityID];
	int rigidBodyIndex = RigidBody::m_Indices[m_ECSManager->m_Cameras[0].m_EntityID];
	glm::vec3 movementDirection = m_ECSManager->m_Transforms[transformIndex].m_Forward;
	glm::vec3 cameraSpeed = m_ECSManager->m_RigidBodies[rigidBodyIndex].m_Velocity;
	glm::vec3 right;
	bool m_YLocked = m_ECSManager->m_Cameras[0].m_YLocked;
	
	if (m_YLocked) movementDirection.y = 0;
	switch (dir)
	{
	case MovementDirection::kForward:
		m_ECSManager->m_Transforms[transformIndex].m_Position += movementDirection * cameraSpeed * delta_time;
		m_ECSManager->m_Transforms[transformIndex+1].m_Position += movementDirection * cameraSpeed * delta_time;
		break;
	case MovementDirection::kBackward:
		m_ECSManager->m_Transforms[transformIndex].m_Position -= movementDirection * cameraSpeed * delta_time;
		m_ECSManager->m_Transforms[transformIndex+1].m_Position -= movementDirection * cameraSpeed * delta_time;
		break;
	case MovementDirection::kRight:
		right = glm::normalize(glm::cross(movementDirection, glm::vec3(0, 1, 0)));
		m_ECSManager->m_Transforms[transformIndex].m_Position += right * delta_time * cameraSpeed;
		m_ECSManager->m_Transforms[transformIndex+1].m_Position += right * delta_time * cameraSpeed;
		break;
	case MovementDirection::kLeft:
		right = glm::normalize(glm::cross(movementDirection, glm::vec3(0, 1, 0)));
		m_ECSManager->m_Transforms[transformIndex].m_Position -= right * delta_time * cameraSpeed;
		m_ECSManager->m_Transforms[transformIndex+1].m_Position -= right * delta_time * cameraSpeed;
		break;
	}
}

void GameLogicSystem::UpdateCameraRotation(int index, double xpos, double ypos, double lastX, double lastY)
{
	//Get the offset of the mouse from last position to move the camera
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	//Update using the mouse sensitivity
	xOffset *= m_ECSManager->m_Cameras[index].m_Sensetivity;
	yOffset *= m_ECSManager->m_Cameras[index].m_Sensetivity;

	//Update the rotation angles
	m_ECSManager->m_Cameras[index].m_Yaw += xOffset;
	m_ECSManager->m_Cameras[index].m_Pitch += yOffset;

	float yaw = m_ECSManager->m_Cameras[index].m_Yaw;
	float pitch = m_ECSManager->m_Cameras[index].m_Pitch;

	m_ECSManager->m_Cameras[index].m_Yaw = yaw >= 360.0f ? yaw - 360.f : yaw;
	m_ECSManager->m_Cameras[index].m_Yaw = yaw <= 0.0f ? yaw + 360.f : yaw;
	m_ECSManager->m_Cameras[index].m_Pitch = pitch >= 89.0f ? 89.0f : pitch;
	m_ECSManager->m_Cameras[index].m_Pitch = pitch <= -89.0f ? -89.0f : pitch;


	int transformIndex = Transform::m_Indices[m_ECSManager->m_Cameras[0].m_EntityID];
	m_ECSManager->m_Transforms[transformIndex].m_Forward.y = sin(glm::radians(m_ECSManager->m_Cameras[index].m_Pitch));
	m_ECSManager->m_Transforms[transformIndex].m_Forward.x = cos(glm::radians(m_ECSManager->m_Cameras[index].m_Pitch)) * cos(glm::radians(m_ECSManager->m_Cameras[index].m_Yaw));
	m_ECSManager->m_Transforms[transformIndex].m_Forward.z = cos(glm::radians(m_ECSManager->m_Cameras[index].m_Pitch)) * sin(glm::radians(m_ECSManager->m_Cameras[index].m_Yaw));
	m_ECSManager->m_Transforms[transformIndex].m_Forward = glm::normalize(m_ECSManager->m_Transforms[transformIndex].m_Forward);
	
}

void GameLogicSystem::UpdateRigidBodyComponents()
{
	for (int i = 0; i < RigidBody::m_Count; i++)
	{
		//Update the velocity using the acceleration
		m_ECSManager->m_RigidBodies[i].m_Velocity += m_ECSManager->m_RigidBodies[i].m_Acceleration;
		//If no acceleration, assume body stopped, assume no inertia and body reaches 0 velocity
		if (glm::length(m_ECSManager->m_RigidBodies[i].m_Acceleration) <= ZERO_ACCELERATION)
			m_ECSManager->m_RigidBodies[i].m_Velocity = glm::vec3(0.0f);
		//Zero the acceleration (Will be re-calculated by events/physical forces)
		m_ECSManager->m_RigidBodies[i].m_Acceleration = glm::vec3(0.0f);
		
		//Cap the speed
		if (m_ECSManager->m_RigidBodies[i].m_Velocity.x > MAX_VELOCITY)
			m_ECSManager->m_RigidBodies[i].m_Velocity.x = MAX_VELOCITY;
		if (m_ECSManager->m_RigidBodies[i].m_Velocity.y > MAX_VELOCITY)
			m_ECSManager->m_RigidBodies[i].m_Velocity.y = MAX_VELOCITY;
		if (m_ECSManager->m_RigidBodies[i].m_Velocity.z > MAX_VELOCITY)
			m_ECSManager->m_RigidBodies[i].m_Velocity.z = MAX_VELOCITY;

	}
}

void GameLogicSystem::DisableDeadEntities()
{
	float time = glfwGetTime();
	for (int i = 0; i < MeshRenderer::m_Count; i++)
	{
		if (m_ECSManager->m_MeshRenderers[i].m_ExplodeStartTime < time - TIME_BEFORE_DESTRUCTION && m_ECSManager->m_MeshRenderers[i].m_ExplodeStartTime != -1)
		{
			int transformIndex = Transform::m_Indices[m_ECSManager->m_MeshRenderers[i].m_EntityID];
			m_ECSManager->m_Transforms[transformIndex].m_Enabled = false;
		}
	}
}

glm::vec3 GameLogicSystem::GetRayCameraNormalized(double xPos, double yPos) const
{
	//Camera data
	float nearPlane = m_ECSManager->m_Cameras[0].m_NearPlane;
	float farPlane = m_ECSManager->m_Cameras[0].m_FarPlane;
	float fov = m_ECSManager->m_Cameras[0].m_FOV;
	int transformIndex = Transform::m_Indices[m_ECSManager->m_Cameras[0].m_EntityID];
	glm::vec3 pos = m_ECSManager->m_Transforms[transformIndex].m_Position;
	glm::vec3 forward = m_ECSManager->m_Transforms[transformIndex].m_Forward;
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(right, forward));
	glm::mat4 viewMatrix = glm::lookAt(pos, pos + forward, up);

	glm::vec4 ray;
	ray.x = (xPos * 2.0f) / (float)m_WindowWidth - 1.0f;
	ray.y = 1.0f - (yPos * 2.0f) / (float)m_WindowHeight;
	ray.z = -1.0f;
	ray.w = 1.0f;
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)m_WindowWidth / (float)m_WindowHeight, nearPlane, farPlane);
	ray = glm::inverse(projection) * ray;
	ray.z = -1.0f;
	ray.w = 0.0f;
	ray = glm::inverse(viewMatrix) * ray;
	glm::vec3 rayNormalized = glm::vec3(ray.x, ray.y, ray.z);
	rayNormalized = glm::normalize(rayNormalized);
	return rayNormalized;
}

int GameLogicSystem::GetRayPickedEntityID(glm::vec3 ray)
{
	int entityId = -1;
	float minT = INT32_MAX;
	for (int i = 0; i < BoxCollider::m_Count; i++)
	{
		if (!m_ECSManager->m_BoxColliders[i].m_Enabled)
			continue;
		bool intersect;
		std::vector<float> ts;
		ts = GetIntersectionParams(m_ECSManager->m_Transforms[0].m_Position, ray, intersect, i);
		if (intersect && ts[0] < minT)
		{
			entityId = m_ECSManager->m_BoxColliders[i].m_EntityID;
			minT = ts[0];
		}
	}
	return entityId;
}

std::vector<float> GameLogicSystem::GetIntersectionParams(glm::vec3 origin, glm::vec3 dir, bool & intersect, int colliderIndex) const
{
	std::vector<float> t = { -1,-1 };
	intersect = false;

	glm::vec3 invDir = glm::vec3(1 / dir.x, 1 / dir.y, 1 / dir.z);

	glm::vec3 & lowerBound = m_ECSManager->m_BoxColliders[colliderIndex].m_Bounds[0];
	glm::vec3 & upperrBound = m_ECSManager->m_BoxColliders[colliderIndex].m_Bounds[1];

	float t0x = (lowerBound.x - origin.x) * invDir.x;
	float t1x = (upperrBound.x - origin.x) * invDir.x;

	if (t0x > t1x) std::swap(t0x, t1x);
	float tmin = t0x;
	float tmax = t1x;

	float t0y = (lowerBound.y - origin.y) * invDir.y;
	float t1y = (upperrBound.y - origin.y) * invDir.y;
	if (t0y > t1y) std::swap(t0y, t1y);
	tmin = std::max(tmin, t0y);
	tmax = std::min(tmax, t1y);


	float t0z = (lowerBound.z - origin.z) * invDir.z;
	float t1z = (upperrBound.z - origin.z) * invDir.z;
	if (t0z > t1z) std::swap(t0z, t1z);
	tmin = std::max(tmin, t0z);
	tmax = std::min(tmax, t1z);

	if (tmin > tmax || tmin < 0)
		return t;


	t[0] = tmin;
	t[1] = tmax;

	intersect = true;

	return t;
}

void GameLogicSystem::ShootEntity(int entityId)
{
	int rendererIndex = MeshRenderer::m_Indices[entityId];
	int colliderIndex = BoxCollider::m_Indices[entityId];
	m_ECSManager->m_MeshRenderers[rendererIndex].m_ExplodeStartTime = glfwGetTime();
	m_ECSManager->m_BoxColliders[colliderIndex].m_Enabled = false;
}
