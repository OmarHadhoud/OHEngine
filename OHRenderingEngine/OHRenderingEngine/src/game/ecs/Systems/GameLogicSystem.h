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


class GameLogicSystem : public System
{
public:
	GameLogicSystem();
	~GameLogicSystem();
	void SetCurrentWindow(GLFWwindow *currentWindow, int width, int height);
	void Update();
	GameState GetGameState() const;
private:
	//Window variables
	GLFWwindow *m_CurrentWindow;
	int m_WindowWidth;
	int m_WindowHeight;
	//Gamestate variables
	GameState m_gameState;
	//Functions
	void ProcessEvent(Event*);
	int GetRayPickedEntityID();
};

#endif // !GAME_LOGIC_SYSTEM_H