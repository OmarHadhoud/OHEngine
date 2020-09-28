#ifndef SCENE_H
#define SCENE_H

#include <sstream>

class ECSManager;

class Scene
{
public:
	Scene();
	~Scene();
	void LoadScene(ECSManager* manager, int sceneNum);
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
};
#endif // !SCENE_H

