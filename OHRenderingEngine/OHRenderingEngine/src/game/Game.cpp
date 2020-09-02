#include <iostream>

#include <glad/glad.h>

#include "Game.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/Debugger.h"

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

	float vertices[] = {
		-0.5f,	 0.5f,	0.0f,	1.0f, 0.0f, 0.0f,	//0
		-0.5f,	-0.5f,	0.0f,	0.0f, 1.0f, 0.0f,	//1
		 0.5f,	-0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	//2
		 0.5f,	 0.5f,	0.0f,	1.0f, 0.0f, 1.0f	//3

	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	Shader simple_shader("res/shaders/simple.vert", "res/shaders/simple.frag");
	VertexArray vao;

	VertexBufferLayout vbl;
	VertexBuffer vb(vertices, 6 * 4 * sizeof(float), GL_STATIC_DRAW);
	
	IndexBuffer ib(indices, 6 * sizeof(unsigned int), GL_STATIC_DRAW);
	vbl.Push<float>(3, false);
	vbl.Push<float>(3, false);
	
	vao.AddBuffer(vb, vbl);

	while (!glfwWindowShouldClose(m_current_window_))
	{
		vao.Bind();
		simple_shader.Use();
		GlCall(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
		GlCall(glClear(GL_COLOR_BUFFER_BIT));
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
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
	//Load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: FAILED TO INITIALIZE GLAD!" << std::endl;
		return -1;
	}
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
