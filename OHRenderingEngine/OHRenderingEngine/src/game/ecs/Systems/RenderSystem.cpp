#include "RenderSystem.h"
#include "game/Game.h"

//==========================================================================//
//===========================Post procesing data===========================//
//========================================================================//
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
const int MULTI_SAMPLES_COUNT = 8;

//==========================================================================//
//===========================Shadow mapping data===========================//
//========================================================================//
glm::vec3 directions[6] = {
	glm::vec3(1,0,0),
	glm::vec3(-1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,-1,0),
	glm::vec3(0,0,1),
	glm::vec3(0,0,-1)
};
glm::vec3 up[6] = {
	glm::vec3(0,-1,0),
	glm::vec3(0,-1,0),
	glm::vec3(0,0,1),
	glm::vec3(0,0,-1),
	glm::vec3(0,-1,0),
	glm::vec3(0,-1,0)
};



RenderSystem::RenderSystem():
	m_MainShader(Shader("res/shaders/simple_lighting.vert", "res/shaders/simple_lighting.frag", "res/shaders/simple_lighting.geom")),
	m_DebugShader(Shader("res/shaders/debug.vert", "res/shaders/debug.frag", "res/shaders/debug.geom")),
	m_BorderShader(Shader("res/shaders/simple_lighting.vert", "res/shaders/border.frag", "res/shaders/simple_lighting.geom")),
	m_PostProcessShader(Shader("res/shaders/post_process.vert", "res/shaders/post_process.frag")),
	m_BloomShader(Shader("res/shaders/post_process.vert", "res/shaders/bloom.frag")),
	m_ShadowShader(Shader("res/shaders/shadow_shader.vert", "res/shaders/shadow_shader.frag")),
	m_CubemmapShadowShader(Shader("res/shaders/shadow_shader_cubemap.vert", "res/shaders/shadow_shader_cubemap.frag", "res/shaders/shadow_shader_cubemap.geom")),
	m_GammaCorrection(2.2f),
	m_Exposure(1.0f)
{
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::SetCurrentWindow(GLFWwindow * currentWindow, int width, int height)
{
	m_CurrentWindow = currentWindow;
	m_WindowWidth = width;
	m_WindowHeight = height;
}

void RenderSystem::CreateSkybox(const char *path, const char* format)
{
	std::string files[6] = { "px","nx","py","ny","pz","nz" };
	std::vector<std::string> cubemapImages(6);
	for (int i = 0; i < 6; i++)
	{
		cubemapImages[i] = path + files[i] + "." + format;
	}
	m_SkyBox = std::make_unique<Skybox>(cubemapImages);
}

void RenderSystem::Setup()
{
	SetupPostProcessing();
	SetupFrameBuffers();
	SetupDepthMaps();
	Renderer::EnableBlending();
	Renderer::SetBlendFactors(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Renderer::DisableBlending();
}


void RenderSystem::Update()
{
	ProcessEvents();
	CheckWindowSize();

	//Update matrices
	UpdateModelMatrices();
	UpdateLightTrasnformationMatrices();

	//Get meshes ready to draw
	int drawableMeshes[MAX_ENTITY_COUNT];
	int solidMeshes[MAX_ENTITY_COUNT];
	int nonSolidMeshes[MAX_ENTITY_COUNT];
	GetDrawableMeshes(drawableMeshes);
	GetSolidMeshes(drawableMeshes, solidMeshes);
	std::map<float, int> semiTransparentMeshes = GetSemiTransparentMeshes(drawableMeshes);
	GetNonSolidMeshes(drawableMeshes, nonSolidMeshes);

	//Get matrices needed for all drawings
	glm::mat4 view = m_FPSCamera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(m_FPSCamera.GetFOV()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 200.0f);
	glm::mat4 model;

	//Set the lightings in the shaders
	SetLights(view);
	//Shadow mapping
	int enabledShadows[MAX_DEPTH_MAPS + MAX_DEPTH_CUBE_MAPS];
	SetShadows(enabledShadows);
	ShadowPass(enabledShadows, solidMeshes, nonSolidMeshes, semiTransparentMeshes);
	

	//Bind to the frame buffer, clear screen, set settings
	m_MultisampledFBO.Bind();
	Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, glm::vec4(0.4f, 0.2f, 0.4f, 1.0f));
	Renderer::EnableDepthTesting();
	Renderer::EnableAntiAliasing();

	//=============Start rendering objects=================


	//Bind to the main shader and set uniforms that are constant per frame
	m_MainShader.Use();
	m_MainShader.SetMat4("view", view);
	m_MainShader.SetMat4("projection", projection);
	m_MainShader.SetFloat("material.shineness", 64);
	m_MainShader.SetBool("material.blinn", true);
	
	Renderer::EnableCulling();
	Renderer::CullFace(GL_BACK);
	DrawNonTransparentObjects(solidMeshes, m_MainShader);

	Renderer::DisableCulling();
	DrawNonTransparentObjects(nonSolidMeshes, m_MainShader);

	Renderer::EnableBlending();
	Renderer::DisableCulling();
	m_SkyBox->Draw(view, projection);

	m_MainShader.Use();
	DrawSemiTransparentObjects(semiTransparentMeshes, m_MainShader);	
	Renderer::DisableBlending();
	
	DownsampleMSBuffer();
	ApplyBloom();
	DrawInBuffer();
}

void RenderSystem::Draw()
{
	glfwSwapBuffers(m_CurrentWindow);
}


void RenderSystem::ProcessEvent(Event* event)
{
	//TODO: Remove this and add it to either Input System or Game logic system
	switch (event->m_EventType)
	{
	case EventType::kMovePlayer:
	{
		MovePlayerEvent* moveEvent = dynamic_cast<MovePlayerEvent*>(event);
		m_FPSCamera.UpdatePosition(moveEvent->m_MovementDirection, Game::m_DeltaTime);
		break;
	}
	case EventType::GL_TEXTURE_WRAP_RotatePlayer:
	{
		RotatePlayerEvent* rotateEvent = dynamic_cast<RotatePlayerEvent*> (event);
		m_FPSCamera.UpdateRotation(rotateEvent->m_MouseXPos, rotateEvent->m_MouseYPos);
		break;
	}
	case EventType::kUpdatePostProcessingParams:
	{
		UpdatePostProcessingParams* updEvent = dynamic_cast<UpdatePostProcessingParams*> (event);
		m_Exposure = updEvent->m_Exposure;
		m_GammaCorrection = updEvent->m_GammaCorrection;
	}
	default:
		break;
	}

}

void RenderSystem::CheckWindowSize()
{
	int glfwWidth, glfwHeight;
	glfwGetWindowSize(m_CurrentWindow, &glfwWidth, &glfwHeight);
	if (glfwWidth != m_WindowWidth || glfwHeight != m_WindowHeight)
		UpdateWindowSize(glfwWidth, glfwHeight);
}

void RenderSystem::SetupPostProcessing()
{
	//Setup post processing VAO
	VertexBufferLayout tmpVBL;
	m_PostProcessVAO.Bind();
	m_PostProcessVBO = VertexBuffer(quadVertices, 6 * 4 * sizeof(float), GL_STATIC_DRAW);
	m_PostProcessIBO = IndexBuffer(quadIndices, 6 * 1 * sizeof(unsigned int), GL_STATIC_DRAW);
	m_PostProcessVBO.Bind();
	m_PostProcessIBO.Bind();
	tmpVBL.Push<float>(2, false);
	tmpVBL.Push<float>(2, false);
	m_PostProcessVBO.SetLayout(tmpVBL);
	m_PostProcessVAO.AddBuffer(m_PostProcessVBO);
}

void RenderSystem::SetupFrameBuffers()
{
	GLenum attach[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	//Multisampled textures needed
	m_MSColorTex.SetType(GL_TEXTURE_2D_MULTISAMPLE);
	m_MSColorTex.Bind();
	m_MSColorTex.SetMultiSamples(MULTI_SAMPLES_COUNT);
	m_MSColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);

	m_MSBrightTex.SetType(GL_TEXTURE_2D_MULTISAMPLE);
	m_MSBrightTex.Bind();
	m_MSBrightTex.SetMultiSamples(MULTI_SAMPLES_COUNT);
	m_MSBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);

	m_MultisampledRBO.Bind();
	m_MultisampledRBO.EnableMultiSampled();
	m_MultisampledRBO.SetMultiSamples(MULTI_SAMPLES_COUNT);
	m_MultisampledRBO.Create(m_WindowWidth, m_WindowHeight, GL_DEPTH_STENCIL_ATTACHMENT);

	//Main framebuffer for drawing
	m_MultisampledFBO.Bind();
	m_MultisampledFBO.AttachRenderObject(m_MultisampledRBO, GL_DEPTH_STENCIL_ATTACHMENT);
	m_MultisampledFBO.AttachTexture(m_MSColorTex, GL_COLOR_ATTACHMENT0);
	m_MultisampledFBO.AttachTexture(m_MSBrightTex, GL_COLOR_ATTACHMENT1);
	glDrawBuffers(2, attach);
	assert(m_MultisampledFBO.IsComplete());

	//Intermediate buffers to be able to make post processing with multisampling
	m_IntermediateColorTex.SetType(GL_TEXTURE_2D);
	m_IntermediateColorTex.Activate(2);
	m_IntermediateColorTex.Bind();
	m_IntermediateColorTex.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_IntermediateColorTex.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
	m_IntermediateColorTex.SetMinFilter(GL_LINEAR);
	m_IntermediateColorTex.SetMagFilter(GL_LINEAR);
	m_IntermediateColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
	
	m_IntermediateBrightTex.SetType(GL_TEXTURE_2D);
	m_IntermediateBrightTex.Activate(3);
	m_IntermediateBrightTex.Bind();
	m_IntermediateBrightTex.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_IntermediateBrightTex.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
	m_IntermediateBrightTex.SetMinFilter(GL_LINEAR);
	m_IntermediateBrightTex.SetMagFilter(GL_LINEAR);
	m_IntermediateBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);

	m_IntermediateRBO.Bind();
	m_IntermediateRBO.Create(m_WindowWidth, m_WindowHeight, GL_DEPTH_STENCIL_ATTACHMENT);

	m_IntermediateFBO.Bind();
	m_IntermediateFBO.AttachRenderObject(m_IntermediateRBO, GL_DEPTH_STENCIL_ATTACHMENT);
	m_IntermediateFBO.AttachTexture(m_IntermediateColorTex, GL_COLOR_ATTACHMENT0);
	m_IntermediateFBO.AttachTexture(m_IntermediateBrightTex, GL_COLOR_ATTACHMENT1);
	glDrawBuffers(2, attach);
	assert(m_IntermediateFBO.IsComplete());

	//Ping pong buffers for bloom effect
	for (int i = 0; i < 2; i++)
	{
		m_BloomTex[i].SetType(GL_TEXTURE_2D);
		m_BloomTex[i].Activate(4);
		m_BloomTex[i].Bind();
		m_BloomTex[i].SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
		m_BloomTex[i].SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
		m_BloomTex[i].SetMinFilter(GL_LINEAR);
		m_BloomTex[i].SetMagFilter(GL_LINEAR);
		m_BloomTex[i].CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
		m_BloomFBO[i].Bind();
		m_BloomFBO[i].AttachTexture(m_BloomTex[i], GL_COLOR_ATTACHMENT0);
		assert(m_BloomFBO[i].IsComplete());
	}

}

void RenderSystem::SetupDepthMaps()
{
	for (int i = 0; i < MAX_DEPTH_MAPS; i++)
	{
		m_DepthMaps[i].SetType(GL_TEXTURE_2D);
		m_DepthMaps[i].Activate(32 - i);
		m_DepthMaps[i].Bind();
		float color[] = { 1.0f,1.0f,1.0f,1.0f };
		m_DepthMaps[i].SetBorderColor(color);
		m_DepthMaps[i].SetWrap(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		m_DepthMaps[i].SetWrap(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		m_DepthMaps[i].SetMinFilter(GL_NEAREST);
		m_DepthMaps[i].SetMagFilter(GL_NEAREST);
		m_DepthMaps[i].CreateTexImage(SHADOW_MAP_TEXTURE_SIZE, SHADOW_MAP_TEXTURE_SIZE, GL_DEPTH_COMPONENT);

		m_ShadowMapFBO[i].Bind();
		m_ShadowMapFBO[i].AttachTexture(m_DepthMaps[i], GL_DEPTH_ATTACHMENT);
		m_ShadowMapFBO[i].DrawBuffer(GL_NONE);
		m_ShadowMapFBO[i].ReadBuffer(GL_NONE);
		assert(m_ShadowMapFBO[i].IsComplete());
		m_ShadowMapFBO[i].Unbind();
	}

	for (int i = 0; i < MAX_DEPTH_CUBE_MAPS; i++)
	{
		m_DepthCubeMaps[i].SetType(GL_TEXTURE_CUBE_MAP);
		m_DepthCubeMaps[i].Activate(32 - i - MAX_DEPTH_MAPS);
		m_DepthCubeMaps[i].Bind();
		m_DepthCubeMaps[i].SetWrap(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		m_DepthCubeMaps[i].SetWrap(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		m_DepthCubeMaps[i].SetWrap(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		m_DepthCubeMaps[i].SetMagFilter(GL_NEAREST);
		m_DepthCubeMaps[i].SetMinFilter(GL_NEAREST);
		m_DepthCubeMaps[i].CreateTexImage(SHADOW_MAP_TEXTURE_SIZE, SHADOW_MAP_TEXTURE_SIZE, GL_DEPTH_COMPONENT);

		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].Bind();
		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].AttachTexture(m_DepthCubeMaps[i], GL_DEPTH_ATTACHMENT, true);
		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].DrawBuffer(GL_NONE);
		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].ReadBuffer(GL_NONE);
		assert(m_ShadowMapFBO[i + MAX_DEPTH_MAPS].IsComplete());
		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].Unbind();
	}
}

void RenderSystem::UpdateWindowSize(int width, int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
	UpdateTexturesSize();
}

void RenderSystem::UpdateTexturesSize()
{
	m_MultisampledRBO.Bind();
	m_MultisampledRBO.Create(m_WindowWidth, m_WindowHeight, GL_DEPTH_STENCIL_ATTACHMENT);
	m_IntermediateRBO.Bind();
	m_IntermediateRBO.Create(m_WindowWidth, m_WindowHeight, GL_DEPTH_STENCIL_ATTACHMENT);
	m_MSColorTex.Bind();
	m_MSColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
	m_IntermediateColorTex.Bind();
	m_IntermediateColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
	m_MSBrightTex.Bind();
	m_MSBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
	m_IntermediateBrightTex.Bind();
	m_IntermediateBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
	m_BloomTex[0].Bind();
	m_BloomTex[0].CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
	m_BloomTex[1].Bind();
	m_BloomTex[1].CreateTexImage(m_WindowWidth, m_WindowHeight, GL_RGB16F);
}

void RenderSystem::DrawNonTransparentObjects(const int * const indices, const Shader &shader)
{
	shader.Use();
	glm::mat4 model;
	int i = 0;
	while(indices[i] != -1)
	{
		int transformIndex = Transform::m_Indices[indices[i]];
		int meshIndex = MeshRenderer::m_Indices[indices[i]];
		model = m_ECSManager->m_Transforms[transformIndex].m_ModelMatrix;
		shader.SetMat4("model", model);
		m_ECSManager->m_MeshRenderers[meshIndex].m_Model->Draw(shader);
		i++;
	}
}


void RenderSystem::DrawSemiTransparentObjects(std::map<float, int>& indices, const Shader &shader)
{
	shader.Use();
	glm::mat4 model;

	for (auto it = indices.rbegin(); it != indices.rend(); it++)
	{
		int transformIndex = Transform::m_Indices[it->second];
		int meshIndex = MeshRenderer::m_Indices[it->second];
		model = m_ECSManager->m_Transforms[transformIndex].m_ModelMatrix;
		shader.SetMat4("model", model);
		m_ECSManager->m_MeshRenderers[meshIndex].m_Model->Draw(shader);
	}
}


void RenderSystem::DownsampleMSBuffer()
{
	//Copying from MS buffer to normal one
	m_MultisampledFBO.Bind(GL_READ_FRAMEBUFFER);
	m_IntermediateFBO.Bind(GL_DRAW_FRAMEBUFFER);
	m_MultisampledFBO.ReadBuffer(GL_COLOR_ATTACHMENT0);
	m_IntermediateFBO.DrawBuffer(GL_COLOR_ATTACHMENT0);
	Renderer::BlitNamedFrameBuffer(m_MultisampledFBO, m_IntermediateFBO, 0, 0, m_WindowWidth, m_WindowHeight, 0, 0, m_WindowWidth, m_WindowHeight);
	m_MultisampledFBO.ReadBuffer(GL_COLOR_ATTACHMENT1);
	m_IntermediateFBO.DrawBuffer(GL_COLOR_ATTACHMENT1);
	Renderer::BlitNamedFrameBuffer(m_MultisampledFBO, m_IntermediateFBO, 0, 0, m_WindowWidth, m_WindowHeight, 0, 0, m_WindowWidth, m_WindowHeight);
}

void RenderSystem::ApplyBloom()
{
	//Make bloom effect
	m_BloomFBO[0].Bind();
	m_PostProcessVAO.Bind();
	Renderer::DisableDepthTesting();
	m_BloomShader.Use();
	m_BloomShader.SetInt("tex", 1);
	m_BloomShader.SetBool("horizontal", 1);

	m_IntermediateBrightTex.Activate(1);
	m_IntermediateBrightTex.Bind();
	Renderer::Draw(m_PostProcessVAO, m_BloomShader, 6 * 1, 0);
	bool vertical = 1;
	for (int i = 0; i < GAUSSIAN_BLUR_COUNT; i++)
	{
		m_BloomShader.SetBool("horizontal", !vertical);
		m_BloomFBO[vertical].Bind();
		m_BloomTex[!vertical].Activate(1);
		m_BloomTex[!vertical].Bind();
		Renderer::Draw(m_PostProcessVAO, m_BloomShader, 6 * 1, 0);
		vertical = !vertical;
	}
	m_BloomTex[1].Activate(1);
	m_BloomTex[1].Bind();
}

void RenderSystem::DrawInBuffer()
{
	////Post processing
	m_MultisampledFBO.Unbind();
	m_PostProcessShader.Use();
	m_IntermediateColorTex.Activate(1);
	m_IntermediateColorTex.Bind();
	m_BloomTex[0].Activate(2);
	m_BloomTex[0].Bind();
	m_PostProcessShader.SetInt("quadTex", 1);
	m_PostProcessShader.SetInt("brightTex", 2);

	//TODO: remove Shadow debugging
//	Texture::Activate(1);
//	m_DepthMaps[0].Bind();
//	m_DepthMaps[0].Activate(2);
//	m_DepthMaps[0].Bind();
	
	//If player is moving, apply blur
	m_PostProcessShader.SetBool("moving", false);
	m_PostProcessShader.SetFloat("blurStrength", 0.0f);
	m_PostProcessShader.SetFloat("gammaCorrection", m_GammaCorrection);
	m_PostProcessShader.SetFloat("exposure", m_Exposure);
	m_PostProcessVAO.Bind();
	Renderer::DisableDepthTesting();
	Renderer::Draw(m_PostProcessVAO, m_PostProcessShader, 6 * 1, 0);
}

void RenderSystem::GetDrawableMeshes(int *indices)
{
	int j = 0;
	for (int i = 0; i < Entity::m_Count; i++)
	{
		if (Transform::m_Indices[i] == -1 || MeshRenderer::m_Indices[i] == -1)
			continue;
		
		int transformIndex = Transform::m_Indices[i];
		int meshIndex = MeshRenderer::m_Indices[i];
		if (!m_ECSManager->m_Transforms[transformIndex].m_Enabled || !m_ECSManager->m_MeshRenderers[meshIndex].m_Enabled)
			continue;

		indices[j++] = i;
	}
	indices[j] = -1;
}

void RenderSystem::GetSolidMeshes(const int * const drawableMeshes, int *solidIndices)
{
	int j = 0;
	for (int i = 0; drawableMeshes[i] != -1; i++)
	{
		int mapIndex = MeshRenderer::m_Indices[drawableMeshes[i]];
		if (m_ECSManager->m_MeshRenderers[mapIndex].m_IsSolid && m_ECSManager->m_MeshRenderers[mapIndex].m_Transparency != Transparency::GL_TEXTURE_WRAP_SemiTransparent)
			solidIndices[j++] = drawableMeshes[i];
	}
	solidIndices[j] = -1;
}


void RenderSystem::GetNonSolidMeshes(const int * const drawableMeshes, int * nonSolidIndices)
{
	int j = 0;
	for (int i = 0; drawableMeshes[i] != -1; i++)
	{
		int mapIndex = MeshRenderer::m_Indices[drawableMeshes[i]];
		if (!m_ECSManager->m_MeshRenderers[mapIndex].m_IsSolid && m_ECSManager->m_MeshRenderers[mapIndex].m_Transparency != Transparency::GL_TEXTURE_WRAP_SemiTransparent)
			nonSolidIndices[j++] = drawableMeshes[i];
	}
	nonSolidIndices[j] = -1;
}

std::map<float, int> RenderSystem::GetSemiTransparentMeshes(const int * const drawableMeshes)
{
	std::map<float, int> indices;
	for (int i = 0; drawableMeshes[i] != -1; i++)
	{
		int mapIndex = MeshRenderer::m_Indices[drawableMeshes[i]];
		if (!(m_ECSManager->m_MeshRenderers[mapIndex].m_Transparency == Transparency::GL_TEXTURE_WRAP_SemiTransparent) )
			continue;
		int transformIndex = Transform::m_Indices[drawableMeshes[i]];
		float distance = glm::length(m_FPSCamera.GetPosition() - m_ECSManager->m_Transforms[transformIndex].m_Position);
		indices[distance] = drawableMeshes[i];
	}
	return indices;
}



void RenderSystem::GetDirectionalLights(int *indices)
{
	int j = 0;
	for (int i = 0; i < Entity::m_Count; i++)
	{
		if (Transform::m_Indices[i] == -1|| DirectionalLight::m_Indices[i] == -1)
			continue;

		int transformIndex = Transform::m_Indices[i];
		int directionalLightIndex = DirectionalLight::m_Indices[i];
		if (!m_ECSManager->m_Transforms[transformIndex].m_Enabled || !m_ECSManager->m_DirectionalLights[directionalLightIndex].m_Enabled)
			continue;

		indices[j++] = i;
	}
	indices[j] = -1;
}

void RenderSystem::GetPointLights(int *indices)
{
	int j = 0;
	for (int i = 0; i < Entity::m_Count; i++)
	{
		if (Transform::m_Indices[i] == -1 || PointLight::m_Indices[i] == -1)
			continue;

		int transformIndex = Transform::m_Indices[i];
		int pointLightIndex = PointLight::m_Indices[i];
		if (!m_ECSManager->m_Transforms[transformIndex].m_Enabled || !m_ECSManager->m_PointLights[pointLightIndex].m_Enabled)
			continue;

		indices[j++] = i;
	}
	indices[j] = -1;
}

void RenderSystem::GetSpotlights(int *indices)
{
	int j = 0;
	for (int i = 0; i < Entity::m_Count; i++)
	{
		if (Transform::m_Indices[i] == -1 || SpotLight::m_Indices[i] == -1)
			continue;

		int transformIndex = Transform::m_Indices[i];
		int spotLightIndex = SpotLight::m_Indices[i];
		if (!m_ECSManager->m_Transforms[transformIndex].m_Enabled || !m_ECSManager->m_SpotLights[spotLightIndex].m_Enabled)
			continue;

		indices[j++] = i;
	}
	indices[j] = -1;
}

void RenderSystem::SetLights(const glm::mat4 &viewMatrix)
{
	int directionalLights[MAX_DIRECTIONAL_LIGHTS_COUNT];
	int pointLights[MAX_POINT_LIGHT_COUNT];
	int spotLights[MAX_SPOTLIGHT_COUNT];

	GetDirectionalLights(directionalLights);
	GetPointLights(pointLights);
	GetSpotlights(spotLights);

	SetDirectionalLights(directionalLights, m_MainShader, viewMatrix);
	SetPointLights(pointLights, m_MainShader, viewMatrix);
	SetSpotLights(spotLights, m_MainShader, viewMatrix);
}

void RenderSystem::SetDirectionalLights(const int * const indices, Shader &shader, const glm::mat4 &viewMatrix)
{
	shader.Use();
	int i = 0;
	while(indices[i] != -1)
	{
		int lightIndex = DirectionalLight::m_Indices[indices[i]];
		int transformIndex = Transform::m_Indices[indices[i]];
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].ambient", m_ECSManager->m_DirectionalLights[lightIndex].m_Ambient);
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].diffuse", m_ECSManager->m_DirectionalLights[lightIndex].m_Diffuse);
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].specular", m_ECSManager->m_DirectionalLights[lightIndex].m_Specular);
		shader.SetVec3("directionalLights[" + std::to_string(i) + "].direction", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_DirectionalLights[lightIndex].m_Direction, 0.0f))); //W = 0 as we only want to rotate the direction, not move it with the camera
		shader.SetVec3("directionalLights[" + std::to_string(i) + "].color", m_ECSManager->m_DirectionalLights[lightIndex].m_Color);
		shader.SetBool("directionalLights[" + std::to_string(i) + "].HasShadow", false);
		i++;
	}
	shader.SetInt("n_DirectionalLights", i);

}

void RenderSystem::SetPointLights(const int * const indices, Shader &shader, const glm::mat4 &viewMatrix)
{
	shader.Use();
	int i = 0;
	while (indices[i] != -1)
	{
		int lightIndex = PointLight::m_Indices[indices[i]];
		int transformIndex = Transform::m_Indices[indices[i]];
		shader.SetFloat("pointLights[" + std::to_string(i) + "].ambient", m_ECSManager->m_PointLights[lightIndex].m_Ambient);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].diffuse", m_ECSManager->m_PointLights[lightIndex].m_Diffuse);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].specular", m_ECSManager->m_PointLights[lightIndex].m_Specular);
		shader.SetVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_Transforms[transformIndex].m_Position, 1.0f)));
		shader.SetVec3("pointLights[" + std::to_string(i) + "].world_position", glm::vec3(m_ECSManager->m_Transforms[transformIndex].m_Position));
		shader.SetVec3("pointLights[" + std::to_string(i) + "].color", m_ECSManager->m_PointLights[lightIndex].m_Color);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kC", m_ECSManager->m_PointLights[lightIndex].m_kConstant);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kL", m_ECSManager->m_PointLights[lightIndex].m_GL_LINEAR);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kQ", m_ECSManager->m_PointLights[lightIndex].m_kQuadratic);
		shader.SetBool("pointLights[" + std::to_string(i) + "].HasShadow", false);
		i++;
	}
	//Setting all samplerCubes that are not used as GLSL requires to.
	for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++)
	{
		shader.SetInt("pointLights[" + std::to_string(i) + "].DepthMap", 32);
	}
	shader.SetInt("n_PointLights", i);
}

void RenderSystem::SetSpotLights(const int * const indices, Shader &shader, const glm::mat4 &viewMatrix)
{
	shader.Use();
	int i = 0;
	while (indices[i] != -1)
	{
		int lightIndex = SpotLight::m_Indices[indices[i]];
		int transformIndex = Transform::m_Indices[indices[i]];
		shader.SetFloat("spotLights[" + std::to_string(i) + "].ambient", m_ECSManager->m_SpotLights[lightIndex].m_Ambient);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].diffuse", m_ECSManager->m_SpotLights[lightIndex].m_Diffuse);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].specular", m_ECSManager->m_SpotLights[lightIndex].m_Specular);
		shader.SetVec3("spotLights[" + std::to_string(i) + "].position", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_Transforms[transformIndex].m_Position, 1.0f))); //W = 1 as we want to translate the position
		shader.SetVec3("spotLights[" + std::to_string(i) + "].direction", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_SpotLights[lightIndex].m_Direction, 0.0f))); //W = 0 as we only want to rotate the direction, not move it with the camera
		shader.SetVec3("spotLights[" + std::to_string(i) + "].color", m_ECSManager->m_SpotLights[lightIndex].m_Color);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kC", m_ECSManager->m_SpotLights[lightIndex].m_kConstant);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kL", m_ECSManager->m_SpotLights[lightIndex].m_GL_LINEAR);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kQ", m_ECSManager->m_SpotLights[lightIndex].m_kQuadratic);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].inner_cutoff", m_ECSManager->m_SpotLights[lightIndex].m_InnerCutoff);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].outer_cutoff", m_ECSManager->m_SpotLights[lightIndex].m_OuterCutoff);
		shader.SetBool("spotLights[" + std::to_string(i) + "].HasShadow", false);
		i++;
	}
	shader.SetInt("n_SpotLights", i);
}

void RenderSystem::SetShadows(int* const enabledShadows)
{
	m_MainShader.Use();
	int shadowCounter = 0;
	for (int i = 0; i < LightShadow::m_Count; i++)
	{
		//If light shadows are disabled, skip this iteration
		if (!m_ECSManager->m_LightShadows[i].m_Enabled)
			continue;
		
		int entityId = m_ECSManager->m_LightShadows[i].m_EntityID;
		int transformIndex = Transform::m_Indices[entityId];
		//If no transform component exists for this light source, skip this iteration
		if (transformIndex == -1) 
			continue;

		int lightIndex;

		lightIndex = DirectionalLight::m_Indices[entityId];
		if (lightIndex!= -1 && m_ECSManager->m_DirectionalLights[lightIndex].m_Enabled)
		{
			m_MainShader.SetMat4("directionalLights[" + std::to_string(DirectionalLight::m_Indices[entityId]) + "].TransformationMatrix", m_ECSManager->m_LightShadows[i].m_TransformationMatrix[0]);
			m_MainShader.SetInt("directionalLights[" + std::to_string(DirectionalLight::m_Indices[entityId]) + "].DepthMap", 32 - shadowCounter);
			m_MainShader.SetBool("directionalLights[" + std::to_string(DirectionalLight::m_Indices[entityId]) + "].HasShadow", true);
			enabledShadows[shadowCounter++] = i;
			continue;
		}
		lightIndex = PointLight::m_Indices[entityId];
		if (lightIndex != -1 && m_ECSManager->m_PointLights[lightIndex].m_Enabled)
		{
			m_MainShader.SetFloat("pointLights[" + std::to_string(PointLight::m_Indices[entityId]) + "].far_plane", m_ECSManager->m_LightShadows[i].m_FarPlane);
			m_MainShader.SetInt("pointLights[" + std::to_string(PointLight::m_Indices[entityId]) + "].DepthMap", 32 - shadowCounter);
			m_MainShader.SetBool("pointLights[" + std::to_string(PointLight::m_Indices[entityId]) + "].HasShadow", true);
			enabledShadows[shadowCounter++] = i;
			continue;
		}
		lightIndex = SpotLight::m_Indices[entityId];
		if (lightIndex != -1 && m_ECSManager->m_SpotLights[lightIndex].m_Enabled)
		{
			m_MainShader.SetMat4("spotLights[" + std::to_string(SpotLight::m_Indices[entityId]) + "].TransformationMatrix", m_ECSManager->m_LightShadows[i].m_TransformationMatrix[0]);
			m_MainShader.SetInt("spotLights[" + std::to_string(SpotLight::m_Indices[entityId]) + "].DepthMap", 32 - shadowCounter);
			m_MainShader.SetBool("spotLights[" + std::to_string(SpotLight::m_Indices[entityId]) + "].HasShadow", true);
			enabledShadows[shadowCounter++] = i;
			continue;
		}
	}
	enabledShadows[shadowCounter++] = -1;
}

void RenderSystem::ShadowPass(const int* const enabledShadows, const int * const solidMeshes, const int * const nonSolidMeshes, std::map<float, int>& transparentMeshes)
{
	Renderer::EnableDepthTesting();
	//Cull Front Face to prevent shadow acne
	Renderer::EnableCulling();
	Renderer::CullFace(GL_FRONT);
	//Update the size of the viewport
	Renderer::ResizeWindow(SHADOW_MAP_TEXTURE_SIZE, SHADOW_MAP_TEXTURE_SIZE);
	

	int normalDepthMapsIndex = 0;
	int cubeDepthMapsIndex = MAX_DEPTH_MAPS;

	int j = 0;
	while(enabledShadows[j]!=-1)
	{
		int i = enabledShadows[j];

		int entityId = m_ECSManager->m_LightShadows[i].m_EntityID;
		if (DirectionalLight::m_Indices[entityId] != -1)
		{
			m_ShadowShader.Use();
			m_ShadowMapFBO[normalDepthMapsIndex].Bind();
			Renderer::Clear(GL_DEPTH_BUFFER_BIT, glm::vec4(0.0f));
			m_ShadowShader.SetMat4("TransformationMatrix", m_ECSManager->m_LightShadows[i].m_TransformationMatrix[0]);
			DrawNonTransparentObjects(solidMeshes, m_ShadowShader);
			DrawNonTransparentObjects(nonSolidMeshes, m_ShadowShader);
			DrawSemiTransparentObjects(transparentMeshes, m_ShadowShader);
			Texture::Activate(32 - j);
			m_DepthMaps[normalDepthMapsIndex++].Bind();
		}
		else if (PointLight::m_Indices[entityId] != -1)
		{
			m_CubemmapShadowShader.Use();
			m_ShadowMapFBO[cubeDepthMapsIndex].Bind();
			Renderer::Clear(GL_DEPTH_BUFFER_BIT, glm::vec4(0.0f));
			int transformIndex = Transform::m_Indices[entityId];
			for(int k = 0; k < 6; k++)
				m_CubemmapShadowShader.SetMat4("TransformationMatrix["+std::to_string(k)+"]", m_ECSManager->m_LightShadows[i].m_TransformationMatrix[k]);
			m_CubemmapShadowShader.SetFloat("FarPlane", m_ECSManager->m_LightShadows[i].m_FarPlane);
			m_CubemmapShadowShader.SetVec3("LightPosition", m_ECSManager->m_Transforms[transformIndex].m_Position);
			DrawNonTransparentObjects(solidMeshes, m_CubemmapShadowShader);
			DrawNonTransparentObjects(nonSolidMeshes, m_CubemmapShadowShader);
			DrawSemiTransparentObjects(transparentMeshes, m_CubemmapShadowShader);
			Texture::Activate(32 - j);
			m_DepthCubeMaps[cubeDepthMapsIndex-MAX_DEPTH_MAPS].Bind();
			cubeDepthMapsIndex++;
		}
		else if (SpotLight::m_Indices[entityId] != -1)
		{
			m_ShadowShader.Use();
			m_ShadowMapFBO[normalDepthMapsIndex].Bind();
			Renderer::Clear(GL_DEPTH_BUFFER_BIT, glm::vec4(0.0f));
			m_ShadowShader.SetMat4("TransformationMatrix", m_ECSManager->m_LightShadows[i].m_TransformationMatrix[0]);
			DrawNonTransparentObjects(solidMeshes, m_ShadowShader);
			DrawNonTransparentObjects(nonSolidMeshes, m_ShadowShader);
			DrawSemiTransparentObjects(transparentMeshes, m_ShadowShader);
			Texture::Activate(32-j);
			m_DepthMaps[normalDepthMapsIndex++].Bind();
		}
		j++;
	}
	Renderer::CullFace(GL_BACK);
	Renderer::DisableCulling();
	Renderer::ResizeWindow(m_WindowWidth, m_WindowHeight);
}


void RenderSystem::UpdateModelMatrices()
{
	glm::mat4 model;
	for (int i = 0; i < Transform::m_Count; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_ECSManager->m_Transforms[i].m_Position);
		model = glm::rotate(model, glm::radians(m_ECSManager->m_Transforms[i].m_RotationAngle), m_ECSManager->m_Transforms[i].m_RotationAxis);
		model = glm::scale(model, m_ECSManager->m_Transforms[i].m_Scale);
		m_ECSManager->m_Transforms[i].m_ModelMatrix = model;
	}
}

void RenderSystem::UpdateLightTrasnformationMatrices()
{
	for (int i = 0; i < LightShadow::m_Count; i++)
	{
		int entityId = m_ECSManager->m_LightShadows[i].m_EntityID;
		if (DirectionalLight::m_Indices[entityId] != -1)
		{
			UpdateDirectionalLightTransformationMatrix(i, entityId);
		}
		else if (PointLight::m_Indices[entityId] != -1)
		{
			UpdatePointLightTransformationMatrix(i, entityId);
		}
		else if (SpotLight::m_Indices[entityId] != -1)
		{
			UpdateSpotLightTransformationMatrix(i, entityId);
		}

	}
}

void RenderSystem::UpdateDirectionalLightTransformationMatrix(int shadowIndex, int entityId)
{
	int directionalLightComponentIndex = DirectionalLight::m_Indices[entityId];
	float frustumSize = m_ECSManager->m_LightShadows[shadowIndex].m_FrustumSize;
	float nearPlane = m_ECSManager->m_LightShadows[shadowIndex].m_NearPlane;
	float farPlane = m_ECSManager->m_LightShadows[shadowIndex].m_FarPlane;
	glm::vec3 dir = m_ECSManager->m_DirectionalLights[directionalLightComponentIndex].m_Direction;
	glm::mat4 viewMatrix = glm::lookAt(-dir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix = glm::ortho(-frustumSize, frustumSize, -frustumSize, frustumSize, nearPlane, farPlane);
	glm::mat4 transformationMatrix = projectionMatrix * viewMatrix;
	m_ECSManager->m_LightShadows[shadowIndex].m_TransformationMatrix = std::vector<glm::mat4>(1, transformationMatrix);
}

void RenderSystem::UpdateSpotLightTransformationMatrix(int shadowIndex, int entityId)
{
	int spotLightComponentIndex = SpotLight::m_Indices[entityId];
	int transformComponentIndex = Transform::m_Indices[entityId];
	float frustumSize = m_ECSManager->m_LightShadows[shadowIndex].m_FrustumSize;
	float nearPlane = m_ECSManager->m_LightShadows[shadowIndex].m_NearPlane;
	float farPlane = m_ECSManager->m_LightShadows[shadowIndex].m_FarPlane;
	glm::vec3 pos = m_ECSManager->m_Transforms[transformComponentIndex].m_Position;
	glm::vec3 dir = m_ECSManager->m_SpotLights[spotLightComponentIndex].m_Direction;
	glm::mat4 viewMatrix = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix = glm::ortho(-frustumSize, frustumSize, -frustumSize, frustumSize, nearPlane, farPlane);
	glm::mat4 transformationMatrix = projectionMatrix * viewMatrix;
	m_ECSManager->m_LightShadows[shadowIndex].m_TransformationMatrix = std::vector<glm::mat4>(1, transformationMatrix);
}

void RenderSystem::UpdatePointLightTransformationMatrix(int shadowIndex, int entityId)
{
	int transformComponentIndex = Transform::m_Indices[entityId];
	float nearPlane = m_ECSManager->m_LightShadows[shadowIndex].m_NearPlane;
	float farPlane = m_ECSManager->m_LightShadows[shadowIndex].m_FarPlane;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), (float)1.0f, nearPlane, farPlane);
	std::vector<glm::mat4> matrices(6);
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 pos = m_ECSManager->m_Transforms[transformComponentIndex].m_Position;
		glm::mat4 viewMatrix = glm::lookAt(pos, pos + directions[i], up[i]);
		glm::mat4 transformationMatrix = projectionMatrix * viewMatrix;
		matrices[i] = transformationMatrix;
	}
	m_ECSManager->m_LightShadows[shadowIndex].m_TransformationMatrix = matrices;
}

