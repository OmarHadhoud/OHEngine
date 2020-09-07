#ifndef RENDERER_H
#define RENDERER_H

#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/RenderBuffer.h"
#include "renderer/FrameBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Camera.h"
#include "renderer/Debugger.h"

#include "enums.h"


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
	//Draws using the passed shader and vertex array
	//vao: The vertex array to be used to draw
	//shader: The shader to be used to draw
	//count: The number of vertices to draw
	//offset: The offset in the vao to draw
	static void Draw(const VertexArray &vao, const Shader &shader, unsigned int count, unsigned int offset);
	static void ResizeWindow(int width, int height);
	//Sets the number of anit aliasing samples
	//n: The number of samples for anti aliasing
	static void SetAntiAliasingSamples(unsigned int n);
	static void EnableAntiAliasing();
	static void DisableAntiAliasing();
	//Tests related functions
	static void EnableDepthTesting();
	static void DisableDepthTesting();
	static void EnableStencilTesting();
	static void DisableStencilTesting();
	static void SetDepthMask(bool mask);
	static void SetDepthFunc(ComparisonFunc fn);
	static void SetStencilMask(unsigned int mask);
	static void SetStencilFunc(ComparisonFunc fn, int ref, unsigned int mask);
	static void SetStencilOp(TestOption stencilFail, TestOption depthFail, TestOption stencilDepthPass);
private:
	Renderer();
	~Renderer();
	static glm::vec4 s_ClearColor;
	static bool s_ClearColorBufferEnabled;
	static bool s_ClearDepthBufferEnabled;
	static bool s_ClearStencilBufferEnabled;
	static bool s_AntiAliasingEnabled;
	static unsigned int s_MultiSamples;
};

#endif // !RENDERER_H
