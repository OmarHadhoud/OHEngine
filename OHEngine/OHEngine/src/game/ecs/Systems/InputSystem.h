#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <memory>
#include <vector>
#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"
#include <GLFW/glfw3.h>

const float TIME_BETWEEN_SHOTS = 0.2f;


class InputSystem : public System
{
public:
	InputSystem();
	~InputSystem();
	void Update();
private:
	//Input variables
	bool m_CursorEnabled;
	double m_LastMouseXPos;
	double m_LastMouseYPos;
	float m_MouseLastClickedTime;
	bool m_LeftMouseReleased;
	bool m_RightMouseReleased;
	bool m_FirstTimeMouseMoves;
	//Functions
	void ProcessEvent(Event*);
	void ProcessInput();
	void CheckMouseMovement();
	void CheckMouseClicks();
	void CheckKeyboardInput();
};

#endif // !INPUT_SYSTEM_H