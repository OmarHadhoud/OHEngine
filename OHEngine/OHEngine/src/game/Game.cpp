#include <iostream>
#include <vector>
#include <cmath>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "dependencies/utils/glm_util.h"
#include "renderer/Renderer.h"
#include "renderer/VertexBuffer.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/IndexBuffer.h"
#include "renderer/Texture.h"
#include "renderer/MultiSampledTexture.h"
#include "renderer/CubeMapTexture.h"
#include "renderer/RenderBuffer.h"
#include "renderer/Model.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

float Game::m_DeltaTime = 0;
float Game::m_CurrentFrame = 0;
float Game::m_LastFrame = 0;

//Global variable needed in GLFW callbacks.
//TODO: Better design, maybe a callback class
Game* g_Game;

Game::Game(): 
	m_WindowWidth(WIDTH), 
	m_WindowHeight(HEIGHT),
	m_Moving(false)
{
	InitializeGLFW(MAJOR, MINOR);
	g_Game = this;
}


Game::~Game()
{
	glfwTerminate();
}

bool Game::GameEnded()
{
	return m_GameEnded;
}


int Game::RunLevel()
{
	//Load scene from scene manager
	m_ECSManager.SetupComponents();
	m_Scene.LoadScene(&m_ECSManager, &*m_RenderSystem, &*m_GUISystem,0);	
	bool levelEnded = false;
	//TODO: REMOVE
	//TURN VSYNC OFF FOR DEBUGGING PURPOSES
	glfwSwapInterval(0);
	
	while(m_GameLogicSystem->GetGameState()== GameState::kLevelRunning)
	{
		//Update timings
		m_CurrentFrame = glfwGetTime();
		m_DeltaTime = m_CurrentFrame - m_LastFrame;
		m_LastFrame = m_CurrentFrame;

		m_InputSystem->Update();
		m_GameLogicSystem->Update();
		//AI.Update();
		m_GUISystem->Update();
		m_RenderSystem->Update();
		m_GUISystem->Draw();
		m_RenderSystem->Draw();
		//Sound.Update();
	}
	m_GameEnded = m_GameLogicSystem->GetGameState() == GameState::kGameExited;
	return 0;
}

int Game::Run()
{
	CreateWindow();
	if (m_CurrentWindow == nullptr)
	{
		std::cout << "ERROR: COULDN'T CREATE WINDOW" << std::endl;
		return -1;
	}
	AssignGLFWCallbacks();
	//Load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: FAILED TO INITIALIZE GLAD!" << std::endl;
		return -1;
	}
	CreateSystems();

	//Assign shared classes with systems
	AssignEventController();
	AssignECSManager();
	AssignWindow();
	//Setup systems
	m_RenderSystem->Setup();
	m_GUISystem->Setup();
	return RunLevel();
}

void Game::InitializeGLFW(int major = 3, int minor = 3) const
{
	glfwInit();
	glfwInitHint(GL_MINOR_VERSION, major);
	glfwInitHint(GL_MAJOR_VERSION, minor);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Game::CreateWindow()
{
	//If we have an open window
	if (m_CurrentWindow != nullptr) return;
	m_CurrentWindow = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Demo Window", nullptr , nullptr);
	if (m_CurrentWindow == nullptr) 
		return;
	glfwMakeContextCurrent(m_CurrentWindow);
	glfwSetInputMode(m_CurrentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::CreateSystems()
{
	m_InputSystem = std::make_unique<InputSystem>();
	m_GameLogicSystem = std::make_unique<GameLogicSystem>();
	m_GUISystem = std::make_unique<GUISystem>();
	m_RenderSystem = std::make_unique<RenderSystem>();
}

void Game::AssignEventController()
{
	m_InputSystem->SetEventsController(&m_EventsController);
	m_GameLogicSystem->SetEventsController(&m_EventsController);
	m_GUISystem->SetEventsController(&m_EventsController);
	m_RenderSystem->SetEventsController(&m_EventsController);
}

void Game::AssignECSManager()
{
	m_InputSystem->SetECSManager(&m_ECSManager);
	m_GameLogicSystem->SetECSManager(&m_ECSManager);
	m_GUISystem->SetECSManager(&m_ECSManager);
	m_RenderSystem->SetECSManager(&m_ECSManager);
}

void Game::AssignWindow()
{
	m_InputSystem->SetCurrentWindow(m_CurrentWindow, m_WindowWidth, m_WindowHeight);
	m_RenderSystem->SetCurrentWindow(m_CurrentWindow, m_WindowWidth, m_WindowHeight);
	m_GUISystem->SetCurrentWindow(m_CurrentWindow, m_WindowWidth, m_WindowHeight);
	m_GameLogicSystem->SetCurrentWindow(m_CurrentWindow, m_WindowWidth, m_WindowHeight);
}


void Game::UpdateWindowSize(int width, int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

}



//GLFW Callback functions

void AssignGLFWCallbacks()
{
	glfwSetWindowSizeCallback(g_Game->m_CurrentWindow, window_size_callback);
}


void window_size_callback(GLFWwindow *window, int width, int height)
{
	g_Game->m_Renderer.ResizeWindow(width, height);
}

