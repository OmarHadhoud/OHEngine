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
	void Update();
	GameState GetGameState() const;
private:
	//Gamestate variables
	GameState m_gameState;
	//Functions
	void ProcessEvent(Event*);
};

#endif // !GAME_LOGIC_SYSTEM_H