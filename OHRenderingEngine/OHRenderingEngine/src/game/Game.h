#ifndef GAME_H
#define GAME_H


#include "renderer/Renderer.h"

#include <GLFW/glfw3.h>

//Game global variables, game settings
//openGL version
const unsigned int MAJOR = 3; 
const unsigned int MINOR = 3;
//Window size
const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;

class Game
{
public:
	Game();
	~Game();
	bool GameEnded();
	int Run();
private:
	Camera m_Camera;
	bool m_GameEnded;
	GLFWwindow *m_CurrentWindow;
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight;
	float m_DeltaTime;
	float m_LastFrame;
	float m_CurrentFrame;
	void InitializeGLFW(int major, int minor) const;
	void CreateWindow();
	int RunLevel();
	void ProcessInput();
	void AssignGLFWCallbacks() const;
	void SetupIMGUI() const;
};

//GLFW callback functions
void window_size_callback(GLFWwindow *window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#endif // !GAME_H
