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
	m_RenderSystem->CreateSkybox("res/textures/", "png");
	//Load scene from scene manager
	m_Scene.LoadScene(&m_ECSManager, 0);
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
	m_CurrentWindow= glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Demo Window", nullptr, nullptr);
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
	m_InputSystem->SetCurrentWindow(m_CurrentWindow);
	m_RenderSystem->SetCurrentWindow(m_CurrentWindow, m_WindowWidth, m_WindowHeight);
	m_GUISystem->SetCurrentWindow(m_CurrentWindow, m_WindowWidth, m_WindowHeight);
}

void Game::ProcessInput()
{
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_CurrentWindow, true);
	/*if (glfwGetKey(m_CurrentWindow, GLFW_KEY_W))
	{
		m_Camera.UpdatePosition(kForward, m_DeltaTime);
		m_Moving = true;
		m_MovingSpeed += 0.005;
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_S))
	{
		m_Camera.UpdatePosition(kBackward, m_DeltaTime);
		m_Moving = true;
		m_MovingSpeed += 0.005;
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_D))
	{
		m_Camera.UpdatePosition(kRight, m_DeltaTime);
		m_Moving = true;
		m_MovingSpeed += 0.005;
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_A))
	{
		m_Camera.UpdatePosition(kLeft, m_DeltaTime);
		m_Moving = true;
		m_MovingSpeed += 0.005;
		return;
	}*/
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_N))
	{
		m_IsDay = false;
		return;
	}
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_P))
	{
		m_IsDay = true;
		return;
	}
	m_MovingSpeed -= 0.05;
}

void Game::CheckColliderClickedOn(glm::vec3 ray)
{
	for (int i = 0; i < m_Colliders.size(); i++)
	{
		BoxCollider *collider = dynamic_cast<BoxCollider*>(m_Colliders[i]);
		if (collider != nullptr)
		{
			bool intersect = false;
			std::vector<float> ts;
			//ts = collider->GetRayIntersection(m_Camera.GetPosition(),ray, intersect);
			m_Bordered = intersect;
			
		}
	}
}




void Game::SetupIMGUI() const
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_CurrentWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
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
	glfwSetCursorPosCallback(g_Game->m_CurrentWindow, cursor_position_callback);
	glfwSetMouseButtonCallback(g_Game->m_CurrentWindow, mouse_click_callback);
}


void window_size_callback(GLFWwindow *window, int width, int height)
{
	g_Game->m_Renderer.ResizeWindow(width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	g_Game->m_Camera.UpdateRotation(xpos, ypos);
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xPos, yPos;
		xPos = g_Game->m_WindowWidth / 2;
		yPos = g_Game->m_WindowHeight / 2;
		//Commented as we are using FPS view without cursor.
		//glfwGetCursorPos(window, &xPos, &yPos);

		glm::vec4 ray;
		ray.x = (xPos * 2.0f) / (float)g_Game->m_WindowWidth - 1.0f;
		ray.y = 1.0f - (yPos * 2.0f) / (float)g_Game->m_WindowHeight;
		ray.z = -1.0f;
		ray.w = 1.0f;
		glm::mat4 projection = glm::perspective(glm::radians(g_Game->m_Camera.GetFOV()), (float)g_Game->m_WindowWidth / (float)g_Game->m_WindowHeight, 0.1f, 200.0f);
		ray = glm::inverse(projection) * ray;
		ray.z = -1.0f;
		ray.w = 0.0f;
		ray = glm::inverse(g_Game->m_Camera.GetViewMatrix()) * ray;
		glm::vec3 ray_normalized = glm::vec3(ray.x, ray.y, ray.z);
		ray_normalized = glm::normalize(ray_normalized);
		g_Game->CheckColliderClickedOn(ray_normalized);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		g_Game->m_Bordered = false;
	}
}
