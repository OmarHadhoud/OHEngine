#ifndef GUI_SYSTEM_H
#define GUI_SYSTEM_H

#include <vector>
#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"
#include <GLFW/glfw3.h>

class GUISystem : public System
{
public:
	GUISystem();
	~GUISystem();
	void SetCurrentWindow(GLFWwindow *currentWindow, int width, int height);
	void Setup();
	void Update();
	void Draw();
private:
	//Window data
	GLFWwindow *m_CurrentWindow;
	int m_WindowWidth;
	int m_WindowHeight;
	//The GUI data
	float m_Exposure;
	float m_GammaCorrection;
	//Last sent data to other systems
	float m_LastSentExposure;
	float m_LastSentGammaCorrection;
	//Functions
	void ProcessEvent(Event*);
	void StartFrame();
	void ChecGL_TEXTURE_WRAP_Sliders();
};

#endif // !GUI_SYSTEM_H