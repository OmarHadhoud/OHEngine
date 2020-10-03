#include "GameLogicSystem.h"
#include <iostream>
#include <glm/glm.hpp>

GameLogicSystem::GameLogicSystem()
{
}


GameLogicSystem::~GameLogicSystem()
{
}

void GameLogicSystem::SetCurrentWindow(GLFWwindow * currentWindow, int width, int height)
{
	m_CurrentWindow = currentWindow;
	m_WindowWidth = width;
	m_WindowHeight = height;
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
		break;
	}
	case EventType::kPlayerShoots:
	{

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
