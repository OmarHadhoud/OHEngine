#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "dependencies/utils/glm_util.h"
#include "renderer/Renderer.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

Camera *g_CurrentCamera;
Game::Game(): m_WindowWidth(WIDTH), m_WindowHeight(HEIGHT)
{
	InitializeGLFW(MAJOR, MINOR);
	g_CurrentCamera = &m_Camera;
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
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 tex_coord;
		int tex_num;
	};
	Vertex vertices[] = {
		//Front face
		glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	1,	//0
		glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	1,	//1
		glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	1,	//2
		glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	1,	//3
		//Back face
		glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	1,	//4
		glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	1,	//5
		glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	1,	//6
		glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	1,	//7
		//Right face
		glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(0.0f,1.0f),	1,	//8
		glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,0.0f),	1,	//9
		glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	1,	//10
		glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f,1.0f),	1,	//11
		//Left face
		glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(0.0f,1.0f),	1,	//12
		glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,0.0f),	1,	//13
		glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	1,	//14
		glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f,1.0f),	1,	//15
		//Upper face
		glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	1,	//16
		glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	1,	//17
		glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	1,	//18
		glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	1,	//19
		//Upper face
		glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	1,	//20
		glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	1,	//21
		glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	1,	//22
		glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	1	//23
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		
		4, 5, 7,
		5, 6, 7,
		
		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23
	};
	VertexArray vao;


	VertexBufferLayout vbl;
	VertexBuffer vb(vertices, 24 * sizeof(Vertex), GL_STATIC_DRAW);
	
	IndexBuffer ib(indices, 6 * 6 * sizeof(unsigned int), GL_STATIC_DRAW);
	vbl.Push<float>(3, false);
	vbl.Push<float>(3, false);
	vbl.Push<float>(2, false);
	vbl.Push<float>(1, false);
	
	vao.AddBuffer(vb, vbl);
	
	Shader simple_shader("res/shaders/simple.vert", "res/shaders/simple.frag");
	stbi_set_flip_vertically_on_load_thread(1);
	Texture tex1("res/textures/3.png");

	tex1.Activate(0);
	tex1.Bind();
	simple_shader.SetInt("texture0", 0);

	glm::vec3 offset[] = { glm::vec3(0.0f), glm::vec3(0.0f) };
	
	m_LastFrame = glfwGetTime();

	while (!glfwWindowShouldClose(m_CurrentWindow))
	{
		//Update time
		m_CurrentFrame = glfwGetTime();
		m_DeltaTime = m_CurrentFrame - m_LastFrame;
		m_LastFrame = m_CurrentFrame;

		//Start new frame for IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//Render IMGUI GUI
		ImGui::Begin("Editor");
		{
			ImGui::SliderFloat("X offset 1", &offset[0].x, -3.0f, 3.0f);
			ImGui::SliderFloat("Y offset 1", &offset[0].y, -3.0f, 3.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();

		//Clear screen using openGL
		Renderer::SetClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		Renderer::EnableClearColor();
		Renderer::EnableClearDepth();
		Renderer::Clear();

		//Transformations
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);
		model = glm::translate(model, offset[0]);
		view = m_Camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(m_Camera.m_FOV), (float)m_WindowWidth/(float)m_WindowHeight, 0.1f, 100.0f);
		simple_shader.SetMat4("model", model);
		simple_shader.SetMat4("view", view);
		simple_shader.SetMat4("projection", projection);

		//Render using openGL
		Renderer::Draw(vao, simple_shader, 6 * 6, 0);

		//Render GUI onto screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_CurrentWindow);
		glfwPollEvents();
		ProcessInput();
	}
	m_GameEnded = true;
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
	SetupIMGUI();
	Renderer::SetAntiAliasingSamples(16);
	Renderer::EnableDepthTesting();
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

void Game::ProcessInput()
{
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_CurrentWindow, true);
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_W))
		m_Camera.UpdatePosition(kForward, m_DeltaTime);
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_S))
		m_Camera.UpdatePosition(kBackward, m_DeltaTime);
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_D))
		m_Camera.UpdatePosition(kRight, m_DeltaTime);
	if (glfwGetKey(m_CurrentWindow, GLFW_KEY_A))
		m_Camera.UpdatePosition(kLeft, m_DeltaTime);

}



void Game::AssignGLFWCallbacks() const
{
	glfwSetWindowSizeCallback(m_CurrentWindow, window_size_callback);
	glfwSetCursorPosCallback(m_CurrentWindow, cursor_position_callback);
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

//GLFW Callback functions
void window_size_callback(GLFWwindow *window, int width, int height)
{
	Renderer::ResizeWindow(width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	g_CurrentCamera->Update(xpos, ypos);
}
