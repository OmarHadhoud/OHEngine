#include "Scene.h"
#include "game/ecs/ECSManager.h"
#include "renderer/Model.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::LoadScene(ECSManager * manager, int sceneNum)
{
	std::string scenePath = "res/scenes/scene" + std::to_string(sceneNum) +".lvl";
	std::ifstream sceneFile;
	//Ensure ifstream objects can throw exceptions
	sceneFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		sceneFile.open(scenePath.c_str());
		//read file buffer into streams
		m_sceneStream << sceneFile.rdbuf();
		//close file handlers
		sceneFile.close();
		//Parse the scene
		std::string tmpString;
		while (m_sceneStream >> tmpString)
		{
			if (tmpString == "ENTITY_START")
				ParseEntity(manager);
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::COULDNT'T LOAD SCENE!" << std::endl;
	}
}

void Scene::ParseEntity(ECSManager* manager)
{
	int entityId = manager->AddEntities(1);
	std::string tmpString;
	while (m_sceneStream >> tmpString)
	{
		if (tmpString == "BOX_COLLIDER")
			ParseBoxCollider(entityId, manager);
		else if (tmpString == "DIRECTIONAL_LIGHT")
			ParseDirectionalLight(entityId, manager);
		else if (tmpString == "CAMERA")
			ParseCamera(entityId, manager);
		else if (tmpString == "LIGHT_SHADOW")
			ParseLightShadow(entityId, manager);
		else if (tmpString == "MESH_RENDERER")
			ParseMeshRenderer(entityId, manager);
		else if (tmpString == "POINT_LIGHT")
			ParsePointLight(entityId, manager);
		else if (tmpString == "RIGID_BODY")
			ParseRigidBody(entityId, manager);
		else if (tmpString == "SPOT_LIGHT")
			ParseSpotLight(entityId, manager);
		else if (tmpString == "TRANSFORM")
			ParseTransform(entityId, manager);
		else if (tmpString == "PLAYER")
			ParsePlayer(entityId, manager);
		else if (tmpString == "HEALTH")
			ParseHealth(entityId, manager);
		else if (tmpString == "ENTITY_END")
			break;
	}
}


void Scene::ParseBoxCollider(int entityId, ECSManager* manager)
{
	glm::vec3 minBound;
	glm::vec3 maxBound;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> minBound.x >> minBound.y >> minBound.z;
	m_sceneStream >> tmp >> tmp >> maxBound.x >> maxBound.y >> maxBound.z;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<BoxCollider>(entityId);
	manager->m_BoxColliders[id].m_Bounds[0] = minBound;
	manager->m_BoxColliders[id].m_Bounds[1] = maxBound;
	manager->m_BoxColliders[id].m_Enabled = enabled;
}

//TODO: Make it generic
void Scene::ParseCamera(int entityId, ECSManager* manager)
{
	float sensetivity;
	float pitch;
	float yaw;
	float fov;
	float nearPlane;
	float farPlane;
	int yLocked;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> sensetivity;
	m_sceneStream >> tmp >> tmp >> pitch;
	m_sceneStream >> tmp >> tmp >> yaw;
	m_sceneStream >> tmp >> tmp >> fov;
	m_sceneStream >> tmp >> tmp >> nearPlane;
	m_sceneStream >> tmp >> tmp >> farPlane;
	m_sceneStream >> tmp >> tmp >> yLocked;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<Camera>(entityId);
	manager->m_Cameras[id].m_Sensetivity = sensetivity;
	manager->m_Cameras[id].m_Pitch = pitch;
	manager->m_Cameras[id].m_Yaw = yaw;
	manager->m_Cameras[id].m_FOV = fov;
	manager->m_Cameras[id].m_NearPlane = nearPlane;
	manager->m_Cameras[id].m_FarPlane = farPlane;
	manager->m_Cameras[id].m_YLocked = yLocked;
	manager->m_Cameras[id].m_Enabled = enabled;
}

void Scene::ParseDirectionalLight(int entityId, ECSManager* manager)
{
	glm::vec3 color;
	glm::vec3 direction;
	float ambient;
	float diffuse;
	float specular;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> color.x >> color.y >> color.z;
	m_sceneStream >> tmp >> tmp >> direction.x >> direction.y >> direction.z;
	m_sceneStream >> tmp >> tmp >> ambient;
	m_sceneStream >> tmp >> tmp >> diffuse;
	m_sceneStream >> tmp >> tmp >> specular;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<DirectionalLight>(entityId);
	manager->m_DirectionalLights[id].m_Color = color;
	manager->m_DirectionalLights[id].m_Direction = direction;
	manager->m_DirectionalLights[id].m_Ambient = ambient;
	manager->m_DirectionalLights[id].m_Diffuse = diffuse;
	manager->m_DirectionalLights[id].m_Specular= specular;
	manager->m_DirectionalLights[id].m_Enabled = enabled;
}

void Scene::ParseLightShadow(int entityId, ECSManager* manager)
{
	float frustumSize;
	float nearPlane;
	float farPlane;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> frustumSize;
	m_sceneStream >> tmp >> tmp >> nearPlane;
	m_sceneStream >> tmp >> tmp >> farPlane;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<LightShadow>(entityId);
	manager->m_LightShadows[id].m_FrustumSize = frustumSize;
	manager->m_LightShadows[id].m_NearPlane = nearPlane;
	manager->m_LightShadows[id].m_FarPlane = farPlane;
	manager->m_LightShadows[id].m_Enabled = enabled;
}

void Scene::ParseMeshRenderer(int entityId, ECSManager* manager)
{
	std::string modelPath;
	glm::vec3 borderColor;
	int borderEnabled;
	int isTransparent;
	int isSolid;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> modelPath;
	m_sceneStream >> tmp >> tmp >> borderColor.x >> borderColor.y >> borderColor.z;
	m_sceneStream >> tmp >> tmp >> borderEnabled;
	m_sceneStream >> tmp >> tmp >> isTransparent;
	m_sceneStream >> tmp >> tmp >> isSolid;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<MeshRenderer>(entityId);
	manager->m_MeshRenderers[id].m_Model = std::make_unique<Model>(modelPath.c_str());
	manager->m_MeshRenderers[id].m_BorderColor = borderColor;
	manager->m_MeshRenderers[id].m_BorderEnabled = borderEnabled;
	manager->m_MeshRenderers[id].m_Transparency = isTransparent == 0 ? Transparency::kNonTransparent : isTransparent == 1 ? Transparency::GL_TEXTURE_WRAP_SemiTransparent : Transparency::GL_TEXTURE_WRAP_Transparent;
	manager->m_MeshRenderers[id].m_IsSolid = isSolid;
	manager->m_MeshRenderers[id].m_ExplodeStartTime = -1;

	manager->m_MeshRenderers[id].m_Enabled = enabled;
	//Set texture settings for transparent/semi transparent objects
	if (isTransparent == 1 || isTransparent == 2)
	{
		manager->m_MeshRenderers[id].m_Model->UpdateTexturesWrap(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		manager->m_MeshRenderers[id].m_Model->UpdateTexturesWrap(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		manager->m_MeshRenderers[id].m_Model->UpdateTexturesWrap(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

void Scene::ParsePointLight(int entityId, ECSManager* manager)
{
	glm::vec3 color;
	float ambient;
	float diffuse;
	float specular;
	float constant;
	float linear;
	float quadratic;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> color.x >> color.y >> color.z;
	m_sceneStream >> tmp >> tmp >> ambient;
	m_sceneStream >> tmp >> tmp >> diffuse;
	m_sceneStream >> tmp >> tmp >> specular;
	m_sceneStream >> tmp >> tmp >> constant;
	m_sceneStream >> tmp >> tmp >> linear;
	m_sceneStream >> tmp >> tmp >> quadratic;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<PointLight>(entityId);
	manager->m_PointLights[id].m_Color = color;
	manager->m_PointLights[id].m_Ambient = ambient;
	manager->m_PointLights[id].m_Diffuse = diffuse;
	manager->m_PointLights[id].m_Specular = specular;
	manager->m_PointLights[id].m_kConstant= constant;
	manager->m_PointLights[id].m_GL_LINEAR= linear;
	manager->m_PointLights[id].m_kQuadratic= quadratic;
	manager->m_PointLights[id].m_Enabled = enabled;
}

void Scene::ParseRigidBody(int entityId, ECSManager* manager)
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float weight;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> velocity.x >> velocity.y >> velocity.z;
	m_sceneStream >> tmp >> tmp >> acceleration.x >> acceleration.y >> acceleration.z;
	m_sceneStream >> tmp >> tmp >> weight;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<RigidBody>(entityId);
	manager->m_RigidBodies[id].m_Velocity = velocity;
	manager->m_RigidBodies[id].m_Acceleration = acceleration;
	manager->m_RigidBodies[id].m_Weight = weight;
	manager->m_RigidBodies[id].m_Enabled = enabled;
}

void Scene::ParseSpotLight(int entityId, ECSManager* manager)
{
	glm::vec3 color;
	glm::vec3 direction;
	float ambient;
	float diffuse;
	float specular;
	float constant;
	float linear;
	float quadratic;
	float innerCutOffAngle;
	float outterCutOffAngle;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> color.x >> color.y >> color.z;
	m_sceneStream >> tmp >> tmp >> direction.x >> direction.y >> direction.z;
	m_sceneStream >> tmp >> tmp >> ambient;
	m_sceneStream >> tmp >> tmp >> diffuse;
	m_sceneStream >> tmp >> tmp >> specular;
	m_sceneStream >> tmp >> tmp >> constant;
	m_sceneStream >> tmp >> tmp >> linear;
	m_sceneStream >> tmp >> tmp >> quadratic;
	m_sceneStream >> tmp >> tmp >> innerCutOffAngle;
	m_sceneStream >> tmp >> tmp >> outterCutOffAngle;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<SpotLight>(entityId);
	manager->m_SpotLights[id].m_Color = color;
	manager->m_SpotLights[id].m_Direction = direction;
	manager->m_SpotLights[id].m_Ambient = ambient;
	manager->m_SpotLights[id].m_Diffuse = diffuse;
	manager->m_SpotLights[id].m_Specular = specular;
	manager->m_SpotLights[id].m_kConstant = constant;
	manager->m_SpotLights[id].m_GL_LINEAR = linear;
	manager->m_SpotLights[id].m_kQuadratic = quadratic;
	manager->m_SpotLights[id].m_InnerCutoff = cos(glm::radians(innerCutOffAngle));
	manager->m_SpotLights[id].m_OuterCutoff = cos(glm::radians(outterCutOffAngle));
	manager->m_SpotLights[id].m_Enabled = enabled;
}

void Scene::ParseTransform(int entityId, ECSManager* manager)
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotationAngles;
	glm::vec3 forward;
	float rotationAngle;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> position.x >> position.y >> position.z;
	m_sceneStream >> tmp >> tmp >> scale.x >> scale.y >> scale.z;
	m_sceneStream >> tmp >> tmp >> rotationAngles.x >> rotationAngles.y >> rotationAngles.z;
	m_sceneStream >> tmp >> tmp >> forward.x >> forward.y >> forward.z;
	m_sceneStream >> tmp >> tmp >> enabled;
	int id = manager->AddComponent<Transform>(entityId);
	manager->m_Transforms[id].m_Position = position;
	manager->m_Transforms[id].m_Scale = scale;
	manager->m_Transforms[id].m_RotationAngles = rotationAngles;
	manager->m_Transforms[id].m_Forward = forward;
	manager->m_Transforms[id].m_Enabled = enabled;
}

void Scene::ParsePlayer(int entityId, ECSManager* manager)
{
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> enabled;
	
	int id = manager->AddComponent<Player>(entityId);
	manager->m_Players[id].m_Enabled = enabled;
}

void Scene::ParseHealth(int entityId, ECSManager* manager)
{
	float value;
	int enabled;

	std::string tmp;
	m_sceneStream >> tmp >> tmp >> value;
	m_sceneStream >> tmp >> tmp >> enabled;

	int id = manager->AddComponent<Health>(entityId);
	manager->m_Health[id].m_Value= value;
	manager->m_Health[id].m_Enabled = enabled;
}
