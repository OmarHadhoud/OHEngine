#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <memory>
#include <vector>
#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"
#include <GLFW/glfw3.h>

class InputSystem : public System
{
public:
	InputSystem();
	~InputSystem();
	void SetCurrentWindow(GLFWwindow *currentWindow);
	void Update();
private:
	//Input variables
	GLFWwindow *m_CurrentWindow;
	double m_LastMouseXPos;
	double m_LastMouseYPos;
	bool m_LeftMouseReleased;
	bool m_RightMouseReleased;
	//Functions
	void ProcessEvent(Event*);
	void ProcessInput();
	void CheckMouseMovement();
	void CheckMouseClicks();
	void CheckKeyboardInput();
};

#endif // !INPUT_SYSTEM_H