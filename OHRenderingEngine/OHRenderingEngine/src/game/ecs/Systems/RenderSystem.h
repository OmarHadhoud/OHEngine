#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <vector>
#include <map>
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
	//Window variables
	GLFWwindow *m_CurrentWindow;
	int m_WindowWidth;
	int m_WindowHeight;
	//Render settings
	float m_GammaCorrection;
	float m_Exposure;
	//Single entities used by renderer
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
	//Setup Functions
	
	void SetupPostProcessing();
	void SetupFrameBuffers();
	void SetupDepthMaps();
	//Update functions
	void ProcessEvent(Event*);
	void CheckWindowSize();
	void UpdateWindowSize(int widht, int height);
	void UpdateTexturesSize();
	//Predrawing functions
	void UpdateModelMatrices();
	void UpdateLightTrasnformationMatrices();
	void UpdateDirectionalLightTransformationMatrix(int shadowIndex, int entityId);
	void UpdateSpotLightTransformationMatrix(int shadowIndex, int entityId);
	void UpdatePointLightTransformationMatrix(int shadowIndex, int entityId);
	//Render pipeline
	void SetLights(const glm::mat4 &viewMatrix);
	void SetDirectionalLights(const int * const indices, Shader &shader,const glm::mat4 &viewMatrix);
	void SetPointLights(const int * const indices, Shader &shader, const glm::mat4 &viewMatrix);
	void SetSpotLights(const int * const indices, Shader &shader, const glm::mat4 &viewMatrix);
	void SetShadows();
	void ShadowPass(const int * const solidMeshes, const int * const nonSolidMeshes, std::map<float, int> &transparentMeshes);
	
	void DrawSolidObjects(const int * const indices, const Shader &shader);
	void DrawNonSolidOjects(const int * const indices, const Shader &shader);
	void DrawSemiTransparentObjects(std::map<float, int> &indices, const Shader &shader);
	void DownsampleMSBuffer();
	void ApplyBloom();
	void DrawInBuffer();
	void GetDrawableMeshes(int *indices);
	void GetSolidMeshes(const int * const drawableMeshes, int *solidIndices);
	void GetNonSolidMeshes(const int * const drawableMeshes, int *nonSolidIndices);
	std::map<float, int> GetSemiTransparentMeshes(const int * const drawableMeshes);
	void GetDirectionalLights(int *indices);
	void GetPointLights(int *indices);
	void GetSpotlights(int *indices);
};

#endif // !RENDER_SYSTEM_H