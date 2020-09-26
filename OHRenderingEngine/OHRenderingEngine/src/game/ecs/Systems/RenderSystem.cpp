#include "RenderSystem.h"


RenderSystem::RenderSystem()
{
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::SetCurrentWindow(GLFWwindow * currentWindow)
{
	m_CurrentWindow = currentWindow;
}


void RenderSystem::Update()
{

}

void RenderSystem::Draw()
{
	glfwSwapBuffers(m_CurrentWindow);
}


void RenderSystem::ProcessEvent(Event*)
{
}
