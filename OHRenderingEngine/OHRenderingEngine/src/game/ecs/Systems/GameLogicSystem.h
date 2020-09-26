#ifndef GAME_LOGIC_SYSTEM_H
#define GAME_LOGIC_SYSTEM_H

#include <vector>
#include <GLFW/glfw3.h>

#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"

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
	GameState m_gameState;
	void ProcessEvent(Event*);
};

#endif // !GAME_LOGIC_SYSTEM_H