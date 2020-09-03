#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "Game.h"
#include "dependencies/utils/glm_util.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/Texture.h"
#include "renderer/Debugger.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

Game::Game()
{
	InitializeGLFW(MAJOR, MINOR);
	m_window_width_ = WIDTH;
	m_window_height_ = HEIGHT;
}


Game::~Game()
{
	glfwTerminate();
	
}

bool Game::GameEnded()
{
	return m_game_ended_;
}


int Game::RunLevel()
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 tex_coord;
	};
	Vertex vertices[] = {
		glm::vec3(-0.5f,0.5f,0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	//0
		glm::vec3(-0.5f,-0.5f,0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	//1
		glm::vec3(0.5f,-0.5f,0.0f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	//2
		glm::vec3(0.5f,0.5f,0.0f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f)	//3
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	Shader simple_shader("res/shaders/simple.vert", "res/shaders/simple.frag");
	stbi_set_flip_vertically_on_load_thread(1);
	Texture tex("res/textures/2.jpg");

	tex.Activate(0);
	tex.Bind();

	VertexArray vao;

	VertexBufferLayout vbl;
	VertexBuffer vb(vertices, 8 * 4 * sizeof(float), GL_STATIC_DRAW);
	
	IndexBuffer ib(indices, 6 * sizeof(unsigned int), GL_STATIC_DRAW);
	vbl.Push<float>(3, false);
	vbl.Push<float>(3, false);
	vbl.Push<float>(2, false);
	
	vao.AddBuffer(vb, vbl);

	while (!glfwWindowShouldClose(m_current_window_))
	{
		//Start new frame for IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Clear screen using openGL
		GlCall(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
		GlCall(glClear(GL_COLOR_BUFFER_BIT));

		ImGui::Text("This is some useful text.");

		vao.Bind();
		simple_shader.Use();

		//Draw using openGL
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		//Render IMGUI frame		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_current_window_);
		glfwPollEvents();
		ProcessInput();
	}
	m_game_ended_ = true;
	return 0;
}

int Game::Run()
{
	CreateWindow();
	if (m_current_window_ == nullptr)
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
	if (m_current_window_ != nullptr) return;
	m_current_window_= glfwCreateWindow(m_window_width_, m_window_height_, "Demo Window", nullptr, nullptr);
	if (m_current_window_ == nullptr) 
		return;
	glfwMakeContextCurrent(m_current_window_);
}

void Game::ProcessInput()
{
	if (glfwGetKey(m_current_window_, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_current_window_, true);
}



void Game::AssignGLFWCallbacks() const
{
	glfwSetWindowSizeCallback(m_current_window_, window_size_callback);
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
	ImGui_ImplGlfw_InitForOpenGL(m_current_window_, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

//GLFW Callback functions
void window_size_callback(GLFWwindow *window, int width, int height)
{
	GlCall(glViewport(0, 0, width, height));
}
