#ifndef GAME_H
#define GAME_H


#include "renderer/Renderer.h"
#include "game/Collider.h"

#include <memory>
#include <GLFW/glfw3.h>


#include "game/Scene.h"
//Include systems
#include "game/ecs/Systems/InputSystem.h"
#include "game/ecs/Systems/GameLogicSystem.h"
#include "game/ecs/Systems/GUISystem.h"
#include "game/ecs/Systems/RenderSystem.h"
#include "game/ecs/Events/EventsController.h"
#include "game/ecs/ECSManager.h"

//Game global variables, game settings
//openGL version
const unsigned int MAJOR = 4; 
const unsigned int MINOR = 0;
//Window size
const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;

class Game
{
public:
	static float m_DeltaTime;
	static float m_CurrentFrame;
	static float m_LastFrame;
	Game();
	~Game();
	//Checks if the game has ended or not.
	bool GameEnded();
	//Runs the game and returns -1 if failed.
	int Run();
private:
	//ECS MANAGER
	ECSManager m_ECSManager;
	Scene m_Scene;
	//Systems
	std::unique_ptr<InputSystem> m_InputSystem;
	std::unique_ptr <GameLogicSystem> m_GameLogicSystem;
	std::unique_ptr <GUISystem> m_GUISystem;
	std::unique_ptr <RenderSystem> m_RenderSystem;
	//Events
	EventsController m_EventsController;
	//Rendering related
	Renderer m_Renderer;
	//Level related
	bool m_GameEnded;
	std::vector<Collider*> m_Colliders;
	//Player related
	bool m_Moving;
	float m_MovingSpeed;
	bool m_Bordered;
	bool m_IsDay;
	//Window related
	GLFWwindow *m_CurrentWindow;
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight;
	//Initializes GLFW with the passed openGL core version.
	void InitializeGLFW(int major, int minor) const;
	void CreateWindow();
	void CreateSystems();
	void AssignEventController();
	void AssignECSManager();
	void AssignWindow();
	//Runs the game level.
	int RunLevel();
	//Setups IMGUI that is used for GUI.
	void SetupIMGUI() const;
	//Updates window size
	void UpdateWindowSize(int width, int height);
	//Assigns all the callbacks written to GLFW.
	friend void AssignGLFWCallbacks();
	//Friend GLFW callback functions
	friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	friend void window_size_callback(GLFWwindow *window, int width, int height);
	friend void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
	//Processes user input from keyboard.
	void ProcessInput();
	//Collision detecion functions
	void CheckColliderClickedOn(glm::vec3 ray);
};

//GLFW Friend functions
void AssignGLFWCallbacks();
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow *window, int width, int height);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
#endif // !GAME_H
