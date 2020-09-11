#ifndef GAME_H
#define GAME_H


#include "renderer/Renderer.h"
#include "game/Skybox.h"

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
	//Checks if the game has ended or not.
	bool GameEnded();
	//Runs the game and returns -1 if failed.
	int Run();
private:
	//Rendering related
	Renderer m_Renderer;
	Camera m_Camera;
	//Level related
	bool m_GameEnded;
	std::unique_ptr<Skybox> m_Skybox;
	//Player related
	bool m_Moving;
	float m_MovingSpeed;
	//Window related
	GLFWwindow *m_CurrentWindow;
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight;
	//Time related
	float m_DeltaTime;
	float m_LastFrame;
	float m_CurrentFrame;
	//Initializes GLFW with the passed openGL core version.
	void InitializeGLFW(int major, int minor) const;
	void CreateWindow();
	//Runs the game level.
	int RunLevel();
	//Processes user input from keyboard.
	void ProcessInput();
	//Assigns all the callbacks written to GLFW.
	void AssignGLFWCallbacks() const;
	//Setups IMGUI that is used for GUI.
	void SetupIMGUI() const;
	//Updates window size
	void UpdateWindowSize(int width, int height);
};

//GLFW callback functions
void window_size_callback(GLFWwindow *window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#endif // !GAME_H
