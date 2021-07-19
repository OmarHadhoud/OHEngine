#ifndef SCENE_H
#define SCENE_H

#include "ecs/Systems/GUISystem.h"
#include "ecs/Systems/RenderSystem.h"

#include <sstream>

class ECSManager;

class Scene
{
public:
	Scene();
	~Scene();
	void LoadScene(ECSManager* manager, RenderSystem* renderSystem, GUISystem* guiSystem, int sceneNum);
private:
	std::stringstream m_sceneStream;
	//UTILITY FUNCTIONS
	void ParseEntity(ECSManager* manager);
	void ParseBoxCollider(int entityId, ECSManager* manager);
	void ParseCamera(int entityId, ECSManager* manager);
	void ParseDirectionalLight(int entityId, ECSManager* manager);
	void ParseLightShadow(int entityId, ECSManager* manager);
	void ParseMeshRenderer(int entityId, ECSManager* manager);
	void ParsePointLight(int entityId, ECSManager* manager);
	void ParseRigidBody(int entityId, ECSManager* manager);
	void ParseSpotLight(int entityId, ECSManager* manager);
	void ParseTransform(int entityId, ECSManager* manager);
	void ParsePlayer(int entityId, ECSManager* manager);
	void ParseHealth(int entityId, ECSManager* manager);
	void ParseConfig(RenderSystem* renderSystem, GUISystem* guiSystem);
	void ParseSkybox(RenderSystem* renderSystem);
	void ParseExposure(GUISystem* guiSystem);
	void ParseGamma(GUISystem* guiSystem);
};
#endif // !SCENE_H

