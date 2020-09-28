#include "GameLogicSystem.h"

#include <iostream>
#include <glm/glm.hpp>

GameLogicSystem::GameLogicSystem()
{
}


GameLogicSystem::~GameLogicSystem()
{
}


void GameLogicSystem::Update()
{
	ProcessEvents();
	glm::mat4 model;
	for (int i = 0; i <Transform::m_Count; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_ECSManager->m_Transforms[i].m_Position);
		model = glm::rotate(model, m_ECSManager->m_Transforms[i].m_RotationAngle, m_ECSManager->m_Transforms[i].m_RotationAxis);
		model = glm::scale(model, m_ECSManager->m_Transforms[i].m_Scale);
		m_ECSManager->m_Transforms[i].m_ModelMatrix = model;
	}
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
	default:
		break;
	}
}
