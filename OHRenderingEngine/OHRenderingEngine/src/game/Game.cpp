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
		glm::vec4 normal_coord;
		int tex_num;
	};
	//Skybox data
	float skyboxVertices[] = 
	{
	-1.0f,  1.0f, -1.0f,	//0
	-1.0f, -1.0f, -1.0f,	//1
	 1.0f, -1.0f, -1.0f,	//2
	 1.0f, -1.0f, -1.0f,	//3
	 1.0f,  1.0f, -1.0f,	//4
	-1.0f,  1.0f, -1.0f,	//5
							
	-1.0f, -1.0f,  1.0f,	//6
	-1.0f, -1.0f, -1.0f,	//7
	-1.0f,  1.0f, -1.0f,	//8
	-1.0f,  1.0f, -1.0f,	//9
	-1.0f,  1.0f,  1.0f,	//10
	-1.0f, -1.0f,  1.0f,	//11
							
	 1.0f, -1.0f, -1.0f,	//12
	 1.0f, -1.0f,  1.0f,	//13
	 1.0f,  1.0f,  1.0f,	//14
	 1.0f,  1.0f,  1.0f,	//15
	 1.0f,  1.0f, -1.0f,	//16
	 1.0f, -1.0f, -1.0f,	//17
							
	-1.0f, -1.0f,  1.0f,	//18
	-1.0f,  1.0f,  1.0f,	//19
	 1.0f,  1.0f,  1.0f,	//20
	 1.0f,  1.0f,  1.0f,	//21
	 1.0f, -1.0f,  1.0f,	//22
	-1.0f, -1.0f,  1.0f,	//23
							
	-1.0f,  1.0f, -1.0f,	//24
	 1.0f,  1.0f, -1.0f,	//25
	 1.0f,  1.0f,  1.0f,	//26
	 1.0f,  1.0f,  1.0f,	//27
	-1.0f,  1.0f,  1.0f,	//28
	-1.0f,  1.0f, -1.0f,	//29
							
	-1.0f, -1.0f, -1.0f,	//30
	-1.0f, -1.0f,  1.0f,	//31
	 1.0f, -1.0f, -1.0f,	//32
	 1.0f, -1.0f, -1.0f,	//33
	-1.0f, -1.0f,  1.0f,	//34
	 1.0f, -1.0f,  1.0f		//35
	};

	unsigned int skyboxIndices[] =
	{
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35
	};

	VertexArray vaoSkybox;


	VertexBufferLayout vblSkybox;
	VertexBuffer vbSkybox(skyboxVertices, 6 * 6 * 3 * sizeof(float), kStaticDraw);

	IndexBuffer ibSkybox(skyboxIndices, 6 * 2 * 3 * sizeof(unsigned int), kStaticDraw);
	vblSkybox.Push<float>(3, false);
	vaoSkybox.AddBuffer(vbSkybox, vblSkybox);

	std::vector<std::string> cubeMapPaths = {
		"res/textures/right.jpg",
		"res/textures/left.jpg",
		"res/textures/top.jpg",
		"res/textures/bottom.jpg",
		"res/textures/front.jpg",
		"res/textures/back.jpg"
	};

	Texture cubeMapTex;
	cubeMapTex.SetType(kCubeMap);
	cubeMapTex.Bind();
	cubeMapTex.LoadCubemapImages(cubeMapPaths);
	cubeMapTex.SetWrap(kS, kClampToEdge);
	cubeMapTex.SetWrap(kR, kClampToEdge);
	cubeMapTex.SetWrap(kT, kClampToEdge);
	cubeMapTex.SetMinFilter(kLinear);
	cubeMapTex.SetMagFilter(kLinear);

	Shader skyboxShader("res/shaders/cubemap.vert", "res/shaders/cubemap.frag");
	skyboxShader.Use();
	skyboxShader.SetInt("skybox", 0);

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

	VertexArray vaoQuad;


	VertexBufferLayout vblQuad;
	VertexBuffer vbQuad(quadVertices, 6 * 4 * sizeof(float), kStaticDraw);

	IndexBuffer ibQuad(quadIndices, 6 * 1 * sizeof(unsigned int), kStaticDraw);
	vblQuad.Push<float>(2, false);
	vblQuad.Push<float>(2, false);
	vaoQuad.AddBuffer(vbQuad, vblQuad);

	Vertex vertices[] = {
		//Front face
		glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	glm::vec4(0.0f,0.0f,1.0f,0.0f),  1,	//0
		glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	glm::vec4(0.0f,0.0f,1.0f,0.0f),  1,	//1
		glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	glm::vec4(0.0f,0.0f,1.0f,0.0f),  1,	//2
		glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	glm::vec4(0.0f,0.0f,1.0f,0.0f),  1,	//3
		//Back face
		glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	glm::vec4(0.0f,0.0f,-1.0f,0.0f),  1,//4
		glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	glm::vec4(0.0f,0.0f,-1.0f,0.0f),  1,//5
		glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	glm::vec4(0.0f,0.0f,-1.0f,0.0f),  1,//6
		glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	glm::vec4(0.0f,0.0f,-1.0f,0.0f),  1,//7
		//Right face
		glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(0.0f,1.0f),	glm::vec4(1.0f,0.0f,0.0f,0.0f),  1,	//8
		glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,0.0f),	glm::vec4(1.0f,0.0f,0.0f,0.0f),  1,	//9
		glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	glm::vec4(1.0f,0.0f,0.0f,0.0f),  1,	//10
		glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f,1.0f),	glm::vec4(1.0f,0.0f,0.0f,0.0f),  1,	//11
		//Left face
		glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(0.0f,1.0f),	glm::vec4(-1.0f,0.0f,0.0f,0.0f),  1,//12
		glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,0.0f),	glm::vec4(-1.0f,0.0f,0.0f,0.0f),  1,//13
		glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	glm::vec4(-1.0f,0.0f,0.0f,0.0f),  1,//14
		glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f,1.0f),	glm::vec4(-1.0f,0.0f,0.0f,0.0f),  1,//15
		//Upper face
		glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	glm::vec4(0.0f,1.0f,0.0f,0.0f),  1,	//16
		glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	glm::vec4(0.0f,1.0f,0.0f,0.0f),  1,	//17
		glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	glm::vec4(0.0f,1.0f,0.0f,0.0f),  1,	//18
		glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	glm::vec4(0.0f,1.0f,0.0f,0.0f),  1,	//19
		//Downwards face																				
		glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	glm::vec4(0.0f,-1.0f,0.0f,0.0f),  1,//20
		glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	glm::vec4(0.0f,-1.0f,0.0f,0.0f),  1,//21
		glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	glm::vec4(0.0f,-1.0f,0.0f,0.0f),  1,//22
		glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	glm::vec4(0.0f,-1.0f,0.0f,0.0f),  1	//23
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
	VertexBuffer vb(vertices, 24 * sizeof(Vertex), kStaticDraw);
	
	IndexBuffer ib(indices, 6 * 6 * sizeof(unsigned int), kStaticDraw);
	vbl.Push<float>(3, false);
	vbl.Push<float>(3, false);
	vbl.Push<float>(2, false);
	vbl.Push<float>(4, false);
	vbl.Push<float>(1, false);
	
	vao.AddBuffer(vb, vbl);
	
	Shader lamp_shader("res/shaders/lamp.vert", "res/shaders/lamp.frag");
	Shader simple_shader("res/shaders/simple_lighting.vert", "res/shaders/simple_lighting.frag");
	Shader border_shader("res/shaders/simple_lighting.vert", "res/shaders/border.frag");
	Shader post_process("res/shaders/post_process.vert", "res/shaders/post_process.frag");
	stbi_set_flip_vertically_on_load_thread(1);
	Texture tex1("res/textures/container.png");
	Texture tex2("res/textures/container_specular.png");

	tex1.SetType(k2D);
	tex1.Activate(0);
	tex1.Bind();
	tex2.SetType(k2D);
	tex2.Activate(1);
	tex2.Bind();

	simple_shader.Use();
	simple_shader.SetInt("material.diffuseMap", 0);
	simple_shader.SetInt("material.specularMap", 1);


	glm::vec3 offset[] = { glm::vec3(0.0f), glm::vec3(0.0f) };
	
	m_LastFrame = glfwGetTime();
	
	//Post processing getting ready by creating custom frame buffer
	
	Texture colorTex;
	colorTex.SetType(k2D);
	colorTex.Activate(2);
	colorTex.Bind();
	colorTex.SetWrap(kS, kRepeat);
	colorTex.SetWrap(kT, kRepeat);
	colorTex.SetMinFilter(kLinear);
	colorTex.SetMagFilter(kLinear);
	colorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColor);
	post_process.Use();
	post_process.SetInt("quadTex", 2);

	RenderBuffer rbo;
	rbo.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);

	FrameBuffer fbo;
	fbo.AttachRenderObject(rbo, kDepthStencilAttach);
	fbo.AttachTexture(colorTex, kColorAttach0);
	if (!fbo.IsComplete())
		return -1;

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
		fbo.Bind();
		vao.Bind();
		Renderer::SetClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		Renderer::EnableClearColorBuffer();
		Renderer::EnableClearDepthBuffer();
		Renderer::EnableClearStencilBuffer();
		Renderer::EnableDepthTesting();
		Renderer::Clear();

		//Transformations
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);
		glm::mat4 normal = glm::mat4(glm::transpose(glm::inverse(model)));
		glm::vec3 lightPos = glm::vec3(-5 /* cos(glfwGetTime())*/, 5/*sin(glfwGetTime())*/, -5);
		glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);
		model = glm::translate(model, offset[0]);
		view = m_Camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(m_Camera.m_FOV), (float)m_WindowWidth/(float)m_WindowHeight, 0.1f, 100.0f);
		simple_shader.Use();
		simple_shader.SetMat4("model", model);
		simple_shader.SetMat4("view", view);
		simple_shader.SetMat4("projection", projection);
		simple_shader.SetMat4("normal", normal);
		simple_shader.SetVec3("viewPos", m_Camera.m_Pos);

		simple_shader.SetFloat("material.shineness", 32);
		simple_shader.SetFloat("pointLights[0].ambient", 0.2f);
		simple_shader.SetFloat("pointLights[0].diffuse", 0.8f);
		simple_shader.SetFloat("pointLights[0].specular",1.0f);
		simple_shader.SetVec3("pointLights[0].position", lightPos);
		simple_shader.SetVec3("spotLights[0].direction", glm::vec3(0,0,-1));
		simple_shader.SetVec3("pointLights[0].color", lightColor);
		simple_shader.SetFloat("pointLights[0].kC", 1);
		simple_shader.SetFloat("pointLights[0].kL", 0.0045f);
		simple_shader.SetFloat("pointLights[0].kQ", 0.00075f);
		simple_shader.SetFloat("spotLights[0].outer_cutoff", cos(glm::radians(35.0f)));
		simple_shader.SetFloat("spotLights[0].inner_cutoff", cos(glm::radians(25.0f)));
		simple_shader.SetInt("n_PointLights", 1);


		//Setup for border (part 1)
		Renderer::EnableStencilTesting();
		Renderer::SetStencilFunc(kAlways, 1, 0xff);
		Renderer::SetStencilMask(0xff);
		Renderer::SetStencilOp(kKeep, kKeep, kReplace);

		
		//Render using openGL
		Renderer::Draw(vao, simple_shader, 6 * 6, 0);

		//Border : TODO: Not working at the moment because of the skybox, should be handled to work with the skybox.
		model = glm::scale(model, glm::vec3(1.05f)); //Border is larger
		border_shader.Use();
		border_shader.SetMat4("model", model);
		border_shader.SetMat4("view", view);
		border_shader.SetMat4("projection", projection);
		border_shader.SetVec3("BorderColor", glm::vec3(0.4,0.4,0.1));

		Renderer::DisableDepthTesting();
		Renderer::SetStencilFunc(kNotEqual, 1, 0xff);
		Renderer::SetStencilMask(0x00);
		Renderer::Draw(vao, border_shader, 6 * 6, 0);
		Renderer::EnableDepthTesting();
		Renderer::SetStencilMask(0xff);
		Renderer::DisableStencilTesting();
		
		//Draw lamp
		lamp_shader.Use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lamp_shader.SetMat4("model", model);
		lamp_shader.SetMat4("view", view);
		lamp_shader.SetMat4("projection", projection);
		lamp_shader.SetVec3("lightColor", lightColor);
		Renderer::Draw(vao, lamp_shader, 6 * 6, 0);
		vao.Unbind();

		//Cubemap
		Renderer::SetDepthFunc(kLEqual);
		skyboxShader.Use();
		skyboxShader.SetMat4("view", glm::mat4(glm::mat3(view)));
		skyboxShader.SetMat4("projection", projection);
		vaoSkybox.Bind();
		cubeMapTex.Activate(0);
		cubeMapTex.Bind();
		Renderer::Draw(vaoSkybox, skyboxShader, 6*6, 0);
		vaoSkybox.Unbind();
		Renderer::SetDepthFunc(kLess);

		////Post processing
		fbo.Unbind();
		post_process.Use();
		vaoQuad.Bind();
		Renderer::DisableDepthTesting();
		Renderer::Draw(vaoQuad, post_process, 6 * 1, 0);
		
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
