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

//Global variable needed in GLFW callbacks.
//TODO: Better design, maybe a callback class
Game* g_Game;

Game::Game(): m_WindowWidth(WIDTH), m_WindowHeight(HEIGHT), m_Moving(false)
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
	//Initialize the level details
	m_Renderer.SetActiveWindow(m_CurrentWindow);
	m_Renderer.EnableBlending();
	m_Renderer.SetBlendFactors(kSrcAlpha, kOneMinusSrcAlpha);
	std::vector<std::string> cubeMapPaths = {
		"res/textures/px.png",
		"res/textures/nx.png",
		"res/textures/py.png",
		"res/textures/ny.png",
		"res/textures/pz.png",
		"res/textures/nz.png"
	};
	m_Skybox = std::make_unique<Skybox>(cubeMapPaths);
	m_LightManager = std::make_unique<LightManager>();
	VertexBufferLayout TempVBL;
	int glfwWidth;
	int glfwHeight;
	m_MovingSpeed = 0;

	//Post procesing data
	float quadVertices[] = 
	{
		-1.0f,  1.0f,	0.0f, 1.0f,		//0
		-1.0f, -1.0f,	0.0f, 0.0f,		//1
		 1.0f,  1.0f,	1.0f, 1.0f,		//2
								   		
		-1.0f, -1.0f,	0.0f, 0.0f,		//3
		 1.0f, -1.0f,	1.0f, 0.0f,		//4
		 1.0f,  1.0f,	1.0f, 1.0f 		//5
	};
	unsigned int quadIndices[] = {
		0, 1, 2,
		3, 4, 5,
	};
	VertexArray PostProcVAO;
	PostProcVAO.Bind();
	VertexBuffer PostProcVBO(quadVertices, 6 * 4 * sizeof(float), kStaticDraw);
	PostProcVBO.Bind();
	IndexBuffer PostProcIBO(quadIndices, 6 * 1 * sizeof(unsigned int), kStaticDraw);
	PostProcIBO.Bind();
	TempVBL.Push<float>(2, false);
	TempVBL.Push<float>(2, false);
	PostProcVBO.SetLayout(TempVBL);
	TempVBL.Clear();
	PostProcVAO.AddBuffer(PostProcVBO);

	//Shaders used
	Shader MainShader("res/shaders/simple_lighting.vert", "res/shaders/simple_lighting.frag", "res/shaders/simple_lighting.geom");
	Shader MainShaderInstanced("res/shaders/simple_lighting_instanced.vert", "res/shaders/simple_lighting.frag", "res/shaders/simple_lighting.geom");
	Shader DebugShader("res/shaders/debug.vert", "res/shaders/debug.frag", "res/shaders/debug.geom");
	Shader BorderShader("res/shaders/simple_lighting.vert", "res/shaders/border.frag", "res/shaders/simple_lighting.geom");
	Shader PostProcessShader("res/shaders/post_process.vert", "res/shaders/post_process.frag");
	
	//Models
	Model VendingMachine("res/objects/vending_machine/untitled.obj");
	BoxCollider VendingMachineCollider;
	glm::mat4 collider_mat(1.0f);
	collider_mat = glm::translate(collider_mat, glm::vec3(8, -4.5f, -5.0f));
	collider_mat = glm::rotate(collider_mat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	collider_mat = glm::scale(collider_mat, glm::vec3(5.0f));
	glm::vec3 mincoll = glm::vec3(collider_mat*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	glm::vec3 maxcoll = mincoll + glm::vec3(2.0f, 10.0f, 10.0f);

	VendingMachineCollider.SetMinBound(mincoll);
	VendingMachineCollider.SetMaxBound(maxcoll);
	m_Colliders.push_back(&VendingMachineCollider);
	Model Ground("res/objects/ground/ground.obj");
	Model Grass("res/objects/grass/grass.obj");
	Grass.UpdateTexturesWrap(kS, kClampToEdge);
	Grass.UpdateTexturesWrap(kR, kClampToEdge);
	Grass.UpdateTexturesWrap(kT, kClampToEdge);
	Model Window("res/objects/window/window.obj");
	Window.UpdateTexturesWrap(kS, kClampToEdge);
	Window.UpdateTexturesWrap(kR, kClampToEdge);
	Window.UpdateTexturesWrap(kT, kClampToEdge);

	//Textures
	stbi_set_flip_vertically_on_load_thread(1);
	
	//Backpack
	Model BackPack("res/objects/backpack/backpack.obj");
	
	//Timing of last frame initialized
	m_LastFrame = glfwGetTime();
	
	//Post processing getting ready by creating custom frame buffer
	
	MultiSampledTexture colorTex;
	colorTex.SetType(k2DMS);
	colorTex.Bind();
	colorTex.SetMultiSamples(16);
	colorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColor);

	RenderBuffer rbo;
	rbo.Bind();
	rbo.EnableMultiSampled();
	rbo.SetMultiSamples(16);
	rbo.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);

	FrameBuffer fbo;
	fbo.Bind();
	fbo.AttachRenderObject(rbo, kDepthStencilAttach);
	fbo.AttachTexture(colorTex, kColorAttach0);
	if (!fbo.IsComplete())
		return -1;

	//Intermediate buffer to be able to make post processing with multisampling
	Texture colorTexInttermediate;
	colorTexInttermediate.SetType(k2D);
	colorTexInttermediate.Activate(2);
	colorTexInttermediate.Bind();
	colorTexInttermediate.SetWrap(kS, kRepeat);
	colorTexInttermediate.SetWrap(kT, kRepeat);
	colorTexInttermediate.SetMinFilter(kLinear);
	colorTexInttermediate.SetMagFilter(kLinear);
	colorTexInttermediate.CreateTexImage(m_WindowWidth, m_WindowHeight, kColor);
	PostProcessShader.Use();
	PostProcessShader.SetInt("quadTex", 2);

	RenderBuffer rboInttermediate;
	rboInttermediate.Bind();
	rboInttermediate.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);

	FrameBuffer fboInttermediate;
	fboInttermediate.Bind();
	fboInttermediate.AttachRenderObject(rboInttermediate, kDepthStencilAttach);
	fboInttermediate.AttachTexture(colorTexInttermediate, kColorAttach0);
	if (!fboInttermediate.IsComplete())
		return -1;

	//Setup scene lights	
	DirectionaLight l0(glm::vec3(0, 0, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, 1.0f, true);
	PointLight l1(glm::vec3(25, 5, 0), glm::vec3(1.0f, 1.0f, 1.0f), 0.2, 0.8f, 1.0f, 1.0f, 0.00045f, 0.00075f, true);
	PointLight l2(glm::vec3(50, 5, 30), glm::vec3(1.0f, 1.0f, 1.0f), 0.2, 0.8f, 1.0f, 1.0f, 0.00045f, 0.00075f, true);
	SpotLight l3(glm::vec3(75, 5, 0), glm::vec3(1, -1, 0), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.8f, 1.0f, 1.0f, 0.00045f, 0.00075f, glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(25.0f)), true);
	

	m_LightManager->AddLight(l0);
	m_LightManager->AddLight(l1);
	m_LightManager->AddLight(l2);
	m_LightManager->AddLight(l3);
	
	float offset;
	glm::vec3 VendingMachinePos = glm::vec3(9, 0.3f, 0.0f);
	glm::vec3 VendingMachinePosOrig = glm::vec3(9, 0.3f, 0.0f);
	bool MovingVM = false;
	bool Explode = false;
	float explodeDis = 0.0f;
	float ExplosionStarted = 0.0f;

	//Start of rendering loop
	while (!glfwWindowShouldClose(m_CurrentWindow))
	{
		//Get the window size, and update buffer if they changed
		glfwGetWindowSize(m_CurrentWindow, &glfwWidth, &glfwHeight);
		if (glfwWidth != m_WindowWidth || glfwHeight != m_WindowHeight)
			//Should be done in a better modular way
		{
			UpdateWindowSize(glfwWidth, glfwHeight);
			rbo.Bind();
			rbo.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);
			rboInttermediate.Bind();
			rboInttermediate.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);
			colorTex.Bind();
			colorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColor);
			colorTexInttermediate.Bind();
			colorTexInttermediate.CreateTexImage(m_WindowWidth, m_WindowHeight, kColor);
		}

		//Update timings
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
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();

		//Clear screen using openGL
		fbo.Bind();
		m_Renderer.EnableDepthTesting();
		m_Renderer.Clear(kColorBufferBit | kDepthBufferBit | kStencilBufferBit, glm::vec4(0.4f, 0.2f, 0.4f, 1.0f));
		//m_Renderer.EnableCulling();

		//Transformations for the vending machine
		if (m_Bordered && !MovingVM)
		{
			MovingVM = true;
			offset = glm::dot(VendingMachinePos - m_Camera.GetPosition(), m_Camera.GetFront());
			Explode = true;
			ExplosionStarted = glfwGetTime();
		}
		else if(!m_Bordered)
			MovingVM = 0;
		if (MovingVM)
		{
			VendingMachinePos = offset*m_Camera.GetFront() + m_Camera.GetPosition();
			VendingMachineCollider.SetMinBound(mincoll+VendingMachinePos-VendingMachinePosOrig);
			VendingMachineCollider.SetMaxBound(maxcoll + VendingMachinePos - VendingMachinePosOrig);
		}
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);
		glm::vec3 lightPos = glm::vec3(10 , 10, -5);
		glm::vec3 lightColor = glm::vec3(0.5+sin(glfwGetTime())/2, 0, 0.5+cos(glfwGetTime())/2);
		view = m_Camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(m_Camera.GetFOV()), (float)m_WindowWidth/(float)m_WindowHeight, 0.1f, 200.0f);
		MainShader.Use();
		MainShader.SetMat4("model", model);
		MainShader.SetMat4("view", view);
		MainShader.SetMat4("projection", projection);
		MainShader.SetFloat("material.shineness", 32);

		//Update lights data
		l1.SetColor(glm::vec3(std::max<float>(-cos(glfwGetTime() * 2), cos(glfwGetTime() * 2)), 0.0f, std::max<float>(-cos(glfwGetTime() * 3), cos(glfwGetTime() * 3))));
		l2.SetColor(glm::vec3(std::max<float>(-sin(glfwGetTime() * 2), sin(glfwGetTime()*2)), 0.0f, std::max<float>(-sin(glfwGetTime() * 3), sin(glfwGetTime()*3))));
		l3.SetDirection(glm::vec3((sin(glfwGetTime()*2)), -1, cos(glfwGetTime() * 5)));
		m_LightManager->SetLight(MainShader, m_Camera.GetViewMatrix());

		//Setup for border (part 1)
		m_Renderer.EnableStencilTesting();
		m_Renderer.SetStencilFunc(kAlways, 1, 0xff);
		m_Renderer.SetStencilMask(0xff);
		m_Renderer.SetStencilOp(kKeep, kKeep, kReplace);


		//Render using openGL
		//Render the vending machine
		model = glm::translate(model, VendingMachinePos);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		MainShader.SetMat4("model", model);
		VendingMachine.Draw(MainShader);

		//Start rendering the Ground but disable border
		m_Renderer.SetStencilMask(0x00);
		MainShader.SetMat4("model", model);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -5.0f, 0.0f));
		MainShader.SetMat4("model", model);
		Ground.Draw(MainShader);

		//Draw backpack
		model = glm::translate(model, glm::vec3(0, 5.0f, 0.0f));
		explodeDis = 0;
		if (Explode)
		{
			explodeDis = 5 * abs(sin(glm::radians((ExplosionStarted - glfwGetTime()) * 300)));
			if (explodeDis >= 4.8f)
				Explode = false;
		}
		MainShader.SetFloat("time", explodeDis);
		MainShader.SetMat4("model", model);
		BackPack.Draw(MainShader);
		MainShader.SetFloat("time", 0);

		//Draw backpack normals
		/*DebugShader.Use();
		DebugShader.SetMat4("model", model);
		DebugShader.SetMat4("view", view);
		DebugShader.SetMat4("projection", projection);
		BackPack.Draw(DebugShader);*/
		model = glm::translate(model, glm::vec3(0, -5.0f, 0.0f));


		//Disable culling for transparent objects
		m_Renderer.DisableCulling();

		//Draw grass
		MainShader.Use();
		model = glm::scale(model, glm::vec3(3));
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MainShader.SetMat4("model", model);
		for (int i = 0; i < 50; i++)
		{
			model = glm::translate(model, glm::vec3(0.1, 0.5, 0.0f));
			MainShader.SetMat4("model", model);
			Grass.Draw(MainShader);
		}
			
		//Enable culling for lights drawn
		//m_Renderer.EnableCulling();
		//Draw lights (for debug purposes only)
		m_LightManager->DrawLights(view, projection);

		//Skybox
		m_Skybox->Draw(view, projection);

		//After drawing opaque models
		//Draw transparent stuff [MUST BE ORDERED FROM FARTHEST TO CLOSEST]
		//Disable culling for semi transparent objects
		m_Renderer.DisableCulling();
		MainShader.Use();
		MainShader.SetMat4("model", model);
		Window.Draw(MainShader);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		MainShader.SetMat4("model", model);
		Window.Draw(MainShader);


		//Drawing the borders for the vending machine
		model = glm::mat4(1.0f);
		model = glm::translate(model, VendingMachinePos);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::scale(model, glm::vec3(1.03f));	//Border is larger
		BorderShader.Use();
		BorderShader.SetMat4("model", model);
		BorderShader.SetMat4("view", view);
		BorderShader.SetMat4("projection", projection);
		BorderShader.SetVec3("BorderColor", glm::vec3(0.9, 0.2, 0.2));
		m_Renderer.SetStencilFunc(kNotEqual, 1, 0xff);
		m_Renderer.SetStencilMask(0x00);
		if(MovingVM)
			VendingMachine.Draw(BorderShader);
		m_Renderer.EnableDepthTesting();
		m_Renderer.SetStencilMask(0xff);
		m_Renderer.DisableStencilTesting();

		//Copying from MS buffer to normal one
		m_Renderer.BlitNamedFrameBuffer(fbo, fboInttermediate, 0, 0, m_WindowWidth, m_WindowHeight, 0, 0, m_WindowWidth, m_WindowHeight);

		////Post processing
		fbo.Unbind();
		PostProcessShader.Use();
		colorTexInttermediate.Activate(2);
		colorTexInttermediate.Bind();
		//If player is moving, apply blur
		PostProcessShader.SetBool("moving", m_Moving);
		PostProcessShader.SetFloat("blurStrength", std::max(m_MovingSpeed, 0.0f));
		PostProcVAO.Bind();
		m_Renderer.DisableDepthTesting();
		m_Renderer.Draw(PostProcVAO, PostProcessShader, 6 * 1, 0);
		
		//Render GUI onto screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_CurrentWindow);
		glfwPollEvents();
		ProcessInput();
		//Update the moving speed
		m_MovingSpeed = std::max(m_MovingSpeed, -0.5f);
		m_MovingSpeed = std::min(m_MovingSpeed, 0.8f);
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
	m_Renderer.EnableAntiAliasing();
	m_Renderer.SetAntiAliasingSamples(4);
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
			bool intersect;
			std::vector<float> ts;
			ts = collider->GetRayIntersection(m_Camera.GetPosition(),ray, intersect);
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
