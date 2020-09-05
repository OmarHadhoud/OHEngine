#ifndef RENDERER_H
#define RENDERER_H

#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/Texture.h"
#include "renderer/Camera.h"
#include "renderer/Debugger.h"


class Renderer
{
public:
	//Clear related functions
	static void SetClearColor(float r, float g, float b, float a);
	static void EnableClearColorBuffer();
	static void DisableClearColorBuffer();
	static void EnableClearDepthBuffer();
	static void DisableClearDepthBuffer();
	static void EnableClearStencilBuffer();
	static void DisableClearStencilBuffer();
	static void Clear();
	//General functions
	static void Draw(const VertexArray &vao, const Shader &shader, unsigned int count, unsigned int offset);
	static void ResizeWindow(int width, int height);
	static void SetAntiAliasingSamples(unsigned int n);
	static void EnableAntiAliasing();
	static void DisableAntiAliasing();
	//Tests related functions
	static void EnableDepthTesting();
	static void DisableDepthTesting();
	static void EnableStencilTesting();
	static void DisableStencilTesting();

private:
	Renderer();
	~Renderer();
	static glm::vec4 s_ClearColor;
	static bool s_ClearColorBufferEnabled;
	static bool s_ClearDepthBufferEnabled;
	static bool s_ClearStencilBufferEnabled;
};

#endif // !RENDERER_H
