#ifndef GAME_H
#define GAME_H


#include "renderer/Renderer.h"

#include <GLFW/glfw3.h>

//Game global variables, game settings
//openGL version
const unsigned int MAJOR = 3; 
const unsigned int MINOR = 3;
//Window size
const unsigned int WIDTH = 720;
const unsigned int HEIGHT = 720;

class Game
{
public:
	Game();
	~Game();
	bool GameEnded();
	int Run();
private:
	Camera m_camera_;
	bool m_game_ended_;
	GLFWwindow *m_current_window_;
	unsigned int m_window_width_;
	unsigned int m_window_height_;
	float m_delta_time_;
	float m_last_time_;
	float m_current_frame_;
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
