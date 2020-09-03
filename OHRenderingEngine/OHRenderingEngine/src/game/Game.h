#ifndef GAME_H
#define GAME_H

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
	bool m_game_ended_;
	GLFWwindow *m_current_window_;
	unsigned int m_window_width_;
	unsigned int m_window_height_;
	void InitializeGLFW(int major, int minor) const;
	void CreateWindow();
	int RunLevel();
	void ProcessInput();
	void AssignGLFWCallbacks() const;
};

//GLFW callback functions
void window_size_callback(GLFWwindow *window, int width, int height);

#endif // !GAME_H
