#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <vector>

#include "renderer/Texture.h"
#include "renderer/MultiSampledTexture.h"
#include "renderer/CubeMapTexture.h"
#include "renderer/RenderBuffer.h"
#include "renderer/FrameBuffer.h"
#include "renderer/Shader.h"

#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"
#include "game/Skybox.h"

//TODO: Implement Camera as a component, to support multiple cameras (for CCTV/Minimap,etc)
#include "renderer/Camera.h"

#include <map>
#include <GLFW/glfw3.h>

const int MAX_DEPTH_MAPS = 20;
const int MAX_DEPTH_CUBE_MAPS = 5;
const int SHADOW_MAP_TEXTURE_SIZE = 2048;
const int GAUSSIAN_BLUR_COUNT = 5;

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();
	void SetCurrentWindow(GLFWwindow *currentWindow, int width, int height);
	void CreateSkybox(const char* path, const char* format);
	void Setup();
	void Update();
	void Draw();
private:
	GLFWwindow *m_CurrentWindow;
	int m_WindowWidth;
	int m_WindowHeight;

	//Render settings
	float m_GammaCorrection;
	float m_Exposure;

	//Single entities
	std::unique_ptr<Skybox> m_SkyBox;
	Camera m_FPSCamera;
	//Textures
	MultiSampledTexture m_MSColorTex;
	MultiSampledTexture m_MSBrightTex;
	Texture m_IntermediateColorTex;
	Texture m_IntermediateBrightTex;
	Texture m_BloomTex[2];
	Texture m_DepthMaps[MAX_DEPTH_MAPS];
	CubeMapTexture m_DepthCubeMaps[MAX_DEPTH_CUBE_MAPS];

	//Renderbuffers
	RenderBuffer m_MultisampledRBO;
	RenderBuffer m_IntermediateRBO;

	//Framebuffers
	FrameBuffer m_MultisampledFBO;
	FrameBuffer m_IntermediateFBO;
	FrameBuffer m_BloomFBO[2];
	FrameBuffer m_ShadowMapFBO[MAX_DEPTH_MAPS + MAX_DEPTH_CUBE_MAPS];

	//Shaders
	Shader m_MainShader;
	Shader m_DebugShader;
	Shader m_BorderShader;
	Shader m_PostProcessShader;
	Shader m_BloomShader;
	Shader m_ShadowShader;
	Shader m_CubemmapShadowShader;

	//Post processing vertex array
	VertexBuffer m_PostProcessVBO;
	IndexBuffer m_PostProcessIBO;
	VertexArray m_PostProcessVAO;

	void SetupPostProcessing();
	void SetupFrameBuffers();
	void SetupDethMaps();

	void ProcessEvent(Event*);
	void CheckWindowSize();

	void UpdateWindowSize(int widht, int height);
	void UpdateTexturesSize();

	//Render pipeline
	void SetLights(const glm::mat4 &viewMatrix);
	void SetDirectionalLights(std::vector<int> &indices, Shader &shader,const glm::mat4 &viewMatrix);
	void SetPointLights(std::vector<int> &indices, Shader &shader, const glm::mat4 &viewMatrix);
	void SetSpotLights(std::vector<int> &indices, Shader &shader, const glm::mat4 &viewMatrix);
	void DrawSolidObjects(std::vector<int> &indices);
	void DrawNonSolidOjects(std::vector<int> &indices);
	void DrawTransparentObjects(std::map<float, int> &indices);
	void DownsampleMSBuffer();
	void ApplyBloom();
	void DrawInBuffer();
	std::vector<int> GetDrawableMeshes();
	std::vector<int> GetSolidMeshes(const std::vector<int> &drawableMeshes);
	std::vector<int> GetNonSolidMeshes(const std::vector<int> &drawableMeshes);
	std::map<float, int> GetTransparentMeshes(const std::vector<int> &drawableMeshes);
	std::vector<int> GetDirectionalLights();
	std::vector<int> GetPointLights();
	std::vector<int> GetSpotlights();

};

#endif // !RENDER_SYSTEM_H