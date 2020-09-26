#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <vector>
#include <GLFW/glfw3.h>

#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();
	void SetCurrentWindow(GLFWwindow *currentWindow);
	void Update();
	void Draw();
private:
	GLFWwindow *m_CurrentWindow;
	void ProcessEvent(Event*);
};

#endif // !RENDER_SYSTEM_H