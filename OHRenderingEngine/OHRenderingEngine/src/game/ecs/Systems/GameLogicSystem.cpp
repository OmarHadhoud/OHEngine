#include "GameLogicSystem.h"

#include <iostream>

GameLogicSystem::GameLogicSystem()
{
}


GameLogicSystem::~GameLogicSystem()
{
}


void GameLogicSystem::Update()
{
	ProcessEvents();
}

GameState GameLogicSystem::GetGameState() const
{
	return m_gameState;
}

void GameLogicSystem::ProcessEvent(Event* event)
{
	switch (event->m_EventType)
	{
	case EventType::kExitGame:
	{
		m_gameState = GameState::kGameExited;
		break;
	}
	case EventType::kRotatePlayer:
	{
		RotatePlayerEvent *e = dynamic_cast<RotatePlayerEvent*>(event);
		std::cout << "Mouse is at pos: (" << e->m_MouseXPos << ", " << e->m_MouseYPos << ")\n";
		break;
	}
	default:
		break;
	}
}
