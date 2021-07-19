#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <vector>
#include <map>
#include "renderer/Texture.h"
#include "renderer/MultiSampledTexture.h"
#include "renderer/CubeMapTexture.h"
#include "renderer/RenderBuffer.h"
#include "renderer/Skybox.h"
#include "renderer/FrameBuffer.h"
#include "renderer/Shader.h"
#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"
#include <GLFW/glfw3.h>

const int MAX_DEPTH_MAPS = 5;
const int MAX_DEPTH_CUBE_MAPS = 1;
const int SHADOW_MAP_TEXTURE_SIZE = 2048;
const int GAUSSIAN_BLUR_COUNT = 5;
const float RECOIL_DURATION = 0.2f;

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();
	void CreateSkybox(const char* path, const char* format);
	void Setup();
	void Update();
	void Draw();
private:
	//Render settings
	float m_GammaCorrection;
	float m_Exposure;
	int m_CurrentCamera;
	//Single entities used by renderer
	std::unique_ptr<Skybox> m_SkyBox;
	Texture m_CrosshairTex;
	Texture m_GunTex;
	Texture m_GunFireTex;
	float m_RecoilStartedTime;
	float m_ShootingFireStartedTime;
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
	Shader m_BoxCollidersShader;
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
	glm::mat4 GetCameraViewMatrix(int index) const;
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
	void SetShadows(int* const enabledShadows);
	void ShadowPass(const int* const enabledShadows, const int * const solidMeshes, const int * const nonSolidMeshes, std::map<float, int> &transparentMeshes);
	
	void DrawNonTransparentObjects(const int * const indices, const Shader &shader);
	void DrawSemiTransparentObjects(std::map<float, int> &indices, const Shader &shader);
	void DrawFPSGun();
	void DownsampleMSBuffer();
	void ApplyBloom();
	void DrawInBuffer();
	void DrawShootingEffects();
	void GetDrawableMeshes(int *indices);
	void GetSolidMeshes(const int * const drawableMeshes, int *solidIndices);
	void GetNonSolidMeshes(const int * const drawableMeshes, int *nonSolidIndices);
	std::map<float, int> GetSemiTransparentMeshes(const int * const drawableMeshes);
	void GetDirectionalLights(int *indices);
	void GetPointLights(int *indices);
	void GetSpotlights(int *indices);
	//Editor stuff
	VertexBuffer m_CollidersVBO;
	VertexArray m_CollidersVAO;
	void UpdateColliders();
	void DrawColliders(glm::mat4 view, glm::mat4 proj);



};

#endif // !RENDER_SYSTEM_H