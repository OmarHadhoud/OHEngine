#include "InputSystem.h"


InputSystem::InputSystem(): m_LastMouseXPos(0), m_LastMouseYPos(0), m_LeftMouseReleased(true), m_RightMouseReleased(true)
{
}


InputSystem::~InputSystem()
{
}


void InputSystem::SetCurrentWindow(GLFWwindow * currentWindow)
{
	m_CurrentWindow = currentWindow;
}

void InputSystem::Update()
{
	ProcessInput();

}


void InputSystem::ProcessEvent(Event*)
{
}

void InputSystem::ProcessInput()
{
	CheckMouseMovement();
	CheckMouseClicks();
	CheckKeyboardInput();
	glfwPollEvents();
	
}

void InputSystem::CheckMouseMovement()
{
	double xPos, yPos;
	glfwGetCursorPos(m_CurrentWindow, &xPos, &yPos);
	if (xPos != m_LastMouseXPos || yPos != m_LastMouseYPos)
	{
		RotatePlayerEvent *e = new RotatePlayerEvent();
		e->m_EventType = EventType::kRotatePlayer;
		e->m_MouseXPos = xPos;
		e->m_MouseYPos = yPos;
		m_EventsController->AddEvent(e);
	}
	m_LastMouseXPos = xPos;
	m_LastMouseYPos = yPos;
}

void InputSystem::CheckMouseClicks()
{
	int leftMouseClick = glfwGetMouseButton(m_CurrentWindow, GLFW_MOUSE_BUTTON_LEFT);
	int rightMouseClick = glfwGetMouseButton(m_CurrentWindow, GLFW_MOUSE_BUTTON_RIGHT);
	double xPos, yPos;
	glfwGetCursorPos(m_CurrentWindow, &xPos, &yPos);

	if (leftMouseClick == GLFW_PRESS && m_LeftMouseReleased)
	{
		m_LeftMouseReleased = false;
		PlayerMouseClickEvent *e = new PlayerMouseClickEvent();
		e->m_EventType = EventType::kPlayerShoots;
		e->m_MouseXPos = xPos;
		e->m_MouseYPos = yPos;
		m_EventsController->AddEvent(e);
	}
	else if (leftMouseClick == GLFW_RELEASE)
		m_LeftMouseReleased = true;
	if (rightMouseClick == GLFW_PRESS && m_RightMouseReleased)
	{
		m_RightMouseReleased = false;
		PlayerMouseClickEvent *e = new PlayerMouseClickEvent();
		e->m_EventType = EventType::kPlayerSelects;
		e->m_MouseXPos = xPos;
		e->m_MouseYPos = yPos;
		m_EventsController->AddEvent(e);
	}
	else if (rightMouseClick == GLFW_RELEASE)
		m_RightMouseReleased = true;
}

void InputSystem::CheckKeyboardInput()
{
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_ESCAPE))
	{
		Event *e = new Event();
		e->m_EventType = EventType::kExitGame;
		m_EventsController->AddEvent(e);
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_W))
	{
		MovePlayerEvent *e = new MovePlayerEvent();
		e->m_EventType = EventType::kMovePlayer;
		e->m_MovementDirection = MovementDirection::kForward;
		m_EventsController->AddEvent(e);
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_S))
	{
		MovePlayerEvent *e = new MovePlayerEvent();
		e->m_EventType = EventType::kMovePlayer;
		e->m_MovementDirection = MovementDirection::kBackward;
		m_EventsController->AddEvent(e);
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_D))
	{
		MovePlayerEvent *e = new MovePlayerEvent();
		e->m_EventType = EventType::kMovePlayer;
		e->m_MovementDirection = MovementDirection::kRight;
		m_EventsController->AddEvent(e);
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_A))
	{
		MovePlayerEvent *e = new MovePlayerEvent();
		e->m_EventType = EventType::kMovePlayer;
		e->m_MovementDirection = MovementDirection::kLeft;
		m_EventsController->AddEvent(e);
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_T))
	{
		Event *e = new Event();
		e->m_EventType = EventType::kToggleDayTime;
		m_EventsController->AddEvent(e);
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_P))
	{
		Event *e = new Event();
		e->m_EventType = EventType::kPauseGame;
		m_EventsController->AddEvent(e);
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_R))
	{
		Event *e = new Event();
		e->m_EventType = EventType::kResumeGame;
		m_EventsController->AddEvent(e);
		return;
	}
}
