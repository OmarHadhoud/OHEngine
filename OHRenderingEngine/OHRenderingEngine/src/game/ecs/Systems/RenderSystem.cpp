#include "RenderSystem.h"
#include "game/Game.h"


//===================================================================//
//========================Post procesing data=======================//
//=================================================================//
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
	SetupDethMaps();
	Renderer::EnableBlending();
	Renderer::SetBlendFactors(kSrcAlpha, kOneMinusSrcAlpha);
	Renderer::DisableBlending();
}


void RenderSystem::Update()
{
	ProcessEvents();
	CheckWindowSize();
	//Get meshes ready to draw
	int drawableMeshes[MAX_ENTITY_COUNT];
	int solidMeshes[MAX_ENTITY_COUNT];
	int nonSolidMeshes[MAX_ENTITY_COUNT];
	GetDrawableMeshes(drawableMeshes);
	GetSolidMeshes(drawableMeshes, solidMeshes);
	std::map<float, int> semiTransparentMeshes = GetSemiTransparentMeshes(drawableMeshes);
	GetNonSolidMeshes(drawableMeshes, nonSolidMeshes);

	//Bind to the frame buffer, clear screen, set settings
	m_MultisampledFBO.Bind();
	Renderer::Clear(kColorBufferBit | kDepthBufferBit | kStencilBufferBit, glm::vec4(0.4f, 0.2f, 0.4f, 1.0f));
	Renderer::EnableDepthTesting();
	Renderer::EnableAntiAliasing();

	//=============Start rendering objects=================

	//Get matrices needed for all drawings
	glm::mat4 view = m_FPSCamera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(m_FPSCamera.GetFOV()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 200.0f);
	glm::mat4 model;

	//Bind to the main shader and set uniforms that are constant per frame
	m_MainShader.Use();
	m_MainShader.SetMat4("view", view);
	m_MainShader.SetMat4("projection", projection);
	m_MainShader.SetFloat("material.shineness", 64);
	m_MainShader.SetBool("material.blinn", true);
	
	SetLights(view);
	
	DrawSolidObjects(solidMeshes);
	DrawNonSolidOjects(nonSolidMeshes);
	DrawSemiTransparentObjects(semiTransparentMeshes);	
	m_SkyBox->Draw(view, projection);
	
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
	case EventType::kRotatePlayer:
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
	m_PostProcessVBO = VertexBuffer(quadVertices, 6 * 4 * sizeof(float), kStaticDraw);
	m_PostProcessIBO = IndexBuffer(quadIndices, 6 * 1 * sizeof(unsigned int), kStaticDraw);
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
	m_MSColorTex.SetType(k2DMS);
	m_MSColorTex.Bind();
	m_MSColorTex.SetMultiSamples(MULTI_SAMPLES_COUNT);
	m_MSColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);

	m_MSBrightTex.SetType(k2DMS);
	m_MSBrightTex.Bind();
	m_MSBrightTex.SetMultiSamples(MULTI_SAMPLES_COUNT);
	m_MSBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);

	m_MultisampledRBO.Bind();
	m_MultisampledRBO.EnableMultiSampled();
	m_MultisampledRBO.SetMultiSamples(MULTI_SAMPLES_COUNT);
	m_MultisampledRBO.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);

	//Main framebuffer for drawing
	m_MultisampledFBO.Bind();
	m_MultisampledFBO.AttachRenderObject(m_MultisampledRBO, kDepthStencilAttach);
	m_MultisampledFBO.AttachTexture(m_MSColorTex, kColorAttach0);
	m_MultisampledFBO.AttachTexture(m_MSBrightTex, kColorAttach1);
	glDrawBuffers(2, attach);
	assert(m_MultisampledFBO.IsComplete());

	//Intermediate buffers to be able to make post processing with multisampling
	m_IntermediateColorTex.SetType(k2D);
	m_IntermediateColorTex.Activate(2);
	m_IntermediateColorTex.Bind();
	m_IntermediateColorTex.SetWrap(kS, kRepeat);
	m_IntermediateColorTex.SetWrap(kT, kRepeat);
	m_IntermediateColorTex.SetMinFilter(kLinear);
	m_IntermediateColorTex.SetMagFilter(kLinear);
	m_IntermediateColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
	
	m_IntermediateBrightTex.SetType(k2D);
	m_IntermediateBrightTex.Activate(3);
	m_IntermediateBrightTex.Bind();
	m_IntermediateBrightTex.SetWrap(kS, kRepeat);
	m_IntermediateBrightTex.SetWrap(kT, kRepeat);
	m_IntermediateBrightTex.SetMinFilter(kLinear);
	m_IntermediateBrightTex.SetMagFilter(kLinear);
	m_IntermediateBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);

	m_IntermediateRBO.Bind();
	m_IntermediateRBO.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);

	m_IntermediateFBO.Bind();
	m_IntermediateFBO.AttachRenderObject(m_IntermediateRBO, kDepthStencilAttach);
	m_IntermediateFBO.AttachTexture(m_IntermediateColorTex, kColorAttach0);
	m_IntermediateFBO.AttachTexture(m_IntermediateBrightTex, kColorAttach1);
	glDrawBuffers(2, attach);
	assert(m_IntermediateFBO.IsComplete());

	//Ping pong buffers for bloom effect
	for (int i = 0; i < 2; i++)
	{
		m_BloomTex[i].SetType(k2D);
		m_BloomTex[i].Activate(4);
		m_BloomTex[i].Bind();
		m_BloomTex[i].SetWrap(kS, kRepeat);
		m_BloomTex[i].SetWrap(kT, kRepeat);
		m_BloomTex[i].SetMinFilter(kLinear);
		m_BloomTex[i].SetMagFilter(kLinear);
		m_BloomTex[i].CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
		m_BloomFBO[i].Bind();
		m_BloomFBO[i].AttachTexture(m_BloomTex[i], kColorAttach0);
		assert(m_BloomFBO[i].IsComplete());
	}

}

void RenderSystem::SetupDethMaps()
{
	for (int i = 0; i < MAX_DEPTH_MAPS; i++)
	{
		m_DepthMaps[i].SetType(k2D);
		m_DepthMaps[i].Activate(1);
		m_DepthMaps[i].Bind();
		float color[] = { 1.0f,1.0f,1.0f,1.0f };
		m_DepthMaps[i].SetBorderColor(color);
		m_DepthMaps[i].SetWrap(kS, kClampToBorder);
		m_DepthMaps[i].SetWrap(kT, kClampToBorder);
		m_DepthMaps[i].SetMinFilter(kNearest);
		m_DepthMaps[i].SetMagFilter(kNearest);
		m_DepthMaps[i].CreateTexImage(SHADOW_MAP_TEXTURE_SIZE, SHADOW_MAP_TEXTURE_SIZE, kDepth);

		m_ShadowMapFBO[i].Bind();
		m_ShadowMapFBO[i].AttachTexture(m_DepthMaps[i], kDepthAttach);
		m_ShadowMapFBO[i].DrawBuffer(kNone);
		m_ShadowMapFBO[i].ReadBuffer(kNone);
		assert(m_ShadowMapFBO[i].IsComplete());
		m_ShadowMapFBO[i].Unbind();
	}

	for (int i = 0; i < MAX_DEPTH_CUBE_MAPS; i++)
	{
		m_DepthCubeMaps[i].SetType(kCubeMap);
		m_DepthCubeMaps[i].Activate(1);
		m_DepthCubeMaps[i].Bind();
		m_DepthCubeMaps[i].SetWrap(kS, kClampToBorder);
		m_DepthCubeMaps[i].SetWrap(kT, kClampToBorder);
		m_DepthCubeMaps[i].SetWrap(kR, kClampToBorder);
		m_DepthCubeMaps[i].SetMagFilter(kNearest);
		m_DepthCubeMaps[i].SetMinFilter(kNearest);
		m_DepthCubeMaps[i].CreateTexImage(SHADOW_MAP_TEXTURE_SIZE, SHADOW_MAP_TEXTURE_SIZE, kDepth);

		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].Bind();
		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].AttachTexture(m_DepthCubeMaps[i], kDepthAttach, true);
		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].DrawBuffer(kNone);
		m_ShadowMapFBO[i + MAX_DEPTH_MAPS].ReadBuffer(kNone);
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
	m_MultisampledRBO.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);
	m_IntermediateRBO.Bind();
	m_IntermediateRBO.Create(m_WindowWidth, m_WindowHeight, kDepthStencil);
	m_MSColorTex.Bind();
	m_MSColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
	m_IntermediateColorTex.Bind();
	m_IntermediateColorTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
	m_MSBrightTex.Bind();
	m_MSBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
	m_IntermediateBrightTex.Bind();
	m_IntermediateBrightTex.CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
	m_BloomTex[0].Bind();
	m_BloomTex[0].CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
	m_BloomTex[1].Bind();
	m_BloomTex[1].CreateTexImage(m_WindowWidth, m_WindowHeight, kColorF);
}

void RenderSystem::DrawSolidObjects(const int * const indices)
{
	glm::mat4 model;
	Renderer::EnableCulling();
	Renderer::CullFace(kBack);
	int i = 0;
	while(indices[i] != -1)
	{
		int transformIndex = m_ECSManager->m_Transforms->m_Map[indices[i]];
		int meshIndex = m_ECSManager->m_MeshRenderers->m_Map[indices[i]];
		model = m_ECSManager->m_Transforms[transformIndex].m_ModelMatrix;
		m_MainShader.SetMat4("model", model);
		m_ECSManager->m_MeshRenderers[meshIndex].m_Model->Draw(m_MainShader);
		i++;
	}
}

void RenderSystem::DrawNonSolidOjects(const int * const indices)
{
	glm::mat4 model;
	Renderer::DisableCulling();
	int i = 0;
	while(indices[i] != -1)
	{
		int transformIndex = m_ECSManager->m_Transforms->m_Map[indices[i]];
		int meshIndex = m_ECSManager->m_MeshRenderers->m_Map[indices[i]];
		model = m_ECSManager->m_Transforms[transformIndex].m_ModelMatrix;
		m_MainShader.SetMat4("model", model);
		m_ECSManager->m_MeshRenderers[meshIndex].m_Model->Draw(m_MainShader);
		i++;
	}
}

void RenderSystem::DrawSemiTransparentObjects(std::map<float, int>& indices)
{
	glm::mat4 model;
	Renderer::EnableBlending();
	Renderer::DisableCulling();
	for (auto it = indices.rbegin(); it != indices.rend(); it++)
	{
		int transformIndex = m_ECSManager->m_Transforms->m_Map[it->second];
		int meshIndex = m_ECSManager->m_MeshRenderers->m_Map[it->second];
		model = m_ECSManager->m_Transforms[transformIndex].m_ModelMatrix;
		m_MainShader.SetMat4("model", model);
		m_ECSManager->m_MeshRenderers[meshIndex].m_Model->Draw(m_MainShader);
	}
	Renderer::DisableBlending();
	Renderer::EnableCulling();
}


void RenderSystem::DownsampleMSBuffer()
{
	//Copying from MS buffer to normal one
	m_MultisampledFBO.Bind(kRead);
	m_IntermediateFBO.Bind(kDraw);
	m_MultisampledFBO.ReadBuffer(kColorAttach0);
	m_IntermediateFBO.DrawBuffer(kColorAttach0);
	Renderer::BlitNamedFrameBuffer(m_MultisampledFBO, m_IntermediateFBO, 0, 0, m_WindowWidth, m_WindowHeight, 0, 0, m_WindowWidth, m_WindowHeight);
	m_MultisampledFBO.ReadBuffer(kColorAttach1);
	m_IntermediateFBO.DrawBuffer(kColorAttach1);
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
		if (Transform::m_Map.find(i) == Transform::m_Map.end() || MeshRenderer::m_Map.find(i) == MeshRenderer::m_Map.end())
			continue;
		
		int transformIndex = Transform::m_Map[i];
		int meshIndex = MeshRenderer::m_Map[i];
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
		int mapIndex = MeshRenderer::m_Map[drawableMeshes[i]];
		if (m_ECSManager->m_MeshRenderers[mapIndex].m_IsSolid && m_ECSManager->m_MeshRenderers[mapIndex].m_Transparency != Transparency::kSemiTransparent)
			solidIndices[j++] = drawableMeshes[i];
	}
	solidIndices[j] = -1;
}


void RenderSystem::GetNonSolidMeshes(const int * const drawableMeshes, int * nonSolidIndices)
{
	int j = 0;
	for (int i = 0; drawableMeshes[i] != -1; i++)
	{
		int mapIndex = MeshRenderer::m_Map[drawableMeshes[i]];
		if (!m_ECSManager->m_MeshRenderers[mapIndex].m_IsSolid && m_ECSManager->m_MeshRenderers[mapIndex].m_Transparency != Transparency::kSemiTransparent)
			nonSolidIndices[j++] = drawableMeshes[i];
	}
	nonSolidIndices[j] = -1;
}

std::map<float, int> RenderSystem::GetSemiTransparentMeshes(const int * const drawableMeshes)
{
	std::map<float, int> indices;
	for (int i = 0; drawableMeshes[i] != -1; i++)
	{
		int mapIndex = MeshRenderer::m_Map[drawableMeshes[i]];
		if (!(m_ECSManager->m_MeshRenderers[mapIndex].m_Transparency == Transparency::kSemiTransparent) )
			continue;
		int transformIndex = Transform::m_Map[drawableMeshes[i]];
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
		if (Transform::m_Map.find(i) == Transform::m_Map.end() || DirectionalLight::m_Map.find(i) == DirectionalLight::m_Map.end())
			continue;

		int transformIndex = Transform::m_Map[i];
		int directionalLightIndex = DirectionalLight::m_Map[i];
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
		if (Transform::m_Map.find(i) == Transform::m_Map.end() || PointLight::m_Map.find(i) == PointLight::m_Map.end())
			continue;

		int transformIndex = Transform::m_Map[i];
		int pointLightIndex = PointLight::m_Map[i];
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
		if (Transform::m_Map.find(i) == Transform::m_Map.end() || SpotLight::m_Map.find(i) == SpotLight::m_Map.end())
			continue;

		int transformIndex = Transform::m_Map[i];
		int spotLightIndex = SpotLight::m_Map[i];
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
		int lightIndex = m_ECSManager->m_DirectionalLights->m_Map[indices[i]];
		int transformIndex = m_ECSManager->m_Transforms->m_Map[indices[i]];
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].ambient", m_ECSManager->m_DirectionalLights[lightIndex].m_Ambient);
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].diffuse", m_ECSManager->m_DirectionalLights[lightIndex].m_Diffuse);
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].specular", m_ECSManager->m_DirectionalLights[lightIndex].m_Specular);
		shader.SetVec3("directionalLights[" + std::to_string(i) + "].direction", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_DirectionalLights[lightIndex].m_Direction, 0.0f))); //W = 0 as we only want to rotate the direction, not move it with the camera
		shader.SetVec3("directionalLights[" + std::to_string(i) + "].color", m_ECSManager->m_DirectionalLights[lightIndex].m_Color);
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
		int lightIndex = m_ECSManager->m_PointLights->m_Map[indices[i]];
		int transformIndex = m_ECSManager->m_Transforms->m_Map[indices[i]];
		shader.SetFloat("pointLights[" + std::to_string(i) + "].ambient", m_ECSManager->m_PointLights[lightIndex].m_Ambient);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].diffuse", m_ECSManager->m_PointLights[lightIndex].m_Diffuse);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].specular", m_ECSManager->m_PointLights[lightIndex].m_Specular);
		shader.SetVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_Transforms[transformIndex].m_Position, 1.0f)));
		shader.SetVec3("pointLights[" + std::to_string(i) + "].world_position", glm::vec3(m_ECSManager->m_Transforms[transformIndex].m_Position));
		shader.SetVec3("pointLights[" + std::to_string(i) + "].color", m_ECSManager->m_PointLights[lightIndex].m_Color);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kC", m_ECSManager->m_PointLights[lightIndex].m_kConstant);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kL", m_ECSManager->m_PointLights[lightIndex].m_kLinear);
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kQ", m_ECSManager->m_PointLights[lightIndex].m_kQuadratic);
		i++;
	}
	//TODO: should be removed and handled in shadows code
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
		int lightIndex = m_ECSManager->m_SpotLights->m_Map[indices[i]];
		int transformIndex = m_ECSManager->m_Transforms->m_Map[indices[i]];
		shader.SetFloat("spotLights[" + std::to_string(i) + "].ambient", m_ECSManager->m_SpotLights[lightIndex].m_Ambient);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].diffuse", m_ECSManager->m_SpotLights[lightIndex].m_Diffuse);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].specular", m_ECSManager->m_SpotLights[lightIndex].m_Specular);
		shader.SetVec3("spotLights[" + std::to_string(i) + "].position", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_Transforms[transformIndex].m_Position, 1.0f))); //W = 1 as we want to translate the position
		shader.SetVec3("spotLights[" + std::to_string(i) + "].direction", glm::vec3(viewMatrix*glm::vec4(m_ECSManager->m_SpotLights[lightIndex].m_Direction, 0.0f))); //W = 0 as we only want to rotate the direction, not move it with the camera
		shader.SetVec3("spotLights[" + std::to_string(i) + "].color", m_ECSManager->m_SpotLights[lightIndex].m_Color);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kC", m_ECSManager->m_SpotLights[lightIndex].m_kConstant);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kL", m_ECSManager->m_SpotLights[lightIndex].m_kLinear);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kQ", m_ECSManager->m_SpotLights[lightIndex].m_kQuadratic);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].inner_cutoff", m_ECSManager->m_SpotLights[lightIndex].m_InnerCutoff);
		shader.SetFloat("spotLights[" + std::to_string(i) + "].outer_cutoff", m_ECSManager->m_SpotLights[lightIndex].m_OuterCutoff);
		i++;
	}
	shader.SetInt("n_SpotLights", i);
}
