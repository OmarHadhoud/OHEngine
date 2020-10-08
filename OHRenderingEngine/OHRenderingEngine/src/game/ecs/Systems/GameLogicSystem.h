#ifndef GAME_LOGIC_SYSTEM_H
#define GAME_LOGIC_SYSTEM_H

#include <vector>
#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"
#include <GLFW/glfw3.h>


enum class GameState 
{
	kLevelRunning,
	kLevelEnded,
	kGameEnded,
	kGameExited
};

//Constant values needed
const int MAX_VELOCITY = 20.0f;
const float ZERO_ACCELERATION = 0.001f;
const float TIME_BEFORE_DESTRUCTION = 0.05f;
const float PI = 3.14159265f;
const glm::vec3 WEAPON_ROTATION = glm::vec3(2.85, -85.0f, 0.0f);



class GameLogicSystem : public System
{
public:
	GameLogicSystem();
	~GameLogicSystem();
	void Update();
	GameState GetGameState() const;
private:
	//Gamestate variables
	GameState m_gameState;
	//Camera logic variables
	bool m_FirstMouseMotion;

	//Functions
	void ProcessEvent(Event*);
	void UpdateCameraPosition(int index, MovementDirection dir, float delta_time);
	void UpdateCameraRotation(int index, double xpos, double ypos, double lastx, double lasty);
	void UpdateRigidBodyComponents();
	void DisableDeadEntities();
	//Util functions
	glm::vec3 GetRayCameraNormalized(double xPos, double yPos) const;
	int GetRayPickedEntityID(glm::vec3 ray);
	std::vector<float> GetIntersectionParams(glm::vec3 origin, glm::vec3 dir, bool &intersect, int colliderIndex) const;
	void ShootEntity(int entityId);
};

#endif // !GAME_LOGIC_SYSTEM_H