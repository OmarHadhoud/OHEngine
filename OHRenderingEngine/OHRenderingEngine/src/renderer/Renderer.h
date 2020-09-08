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
#include "renderer/CubeMapTexture.h"
#include "renderer/MultiSampledTexture.h"
#include "renderer/Camera.h"
#include "renderer/Debugger.h"

#include "enums.h"

#include <GLFW/glfw3.h>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void SetActiveWindow(GLFWwindow *&window);
	//Clear related functions
	 void SetClearColor(float r, float g, float b, float a);
	 void EnableClearColorBuffer();
	 void DisableClearColorBuffer();
	 void EnableClearDepthBuffer();
	 void DisableClearDepthBuffer();
	 void EnableClearStencilBuffer();
	 void DisableClearStencilBuffer();
	 void Clear();
	//General functions
	//Draws using the passed shader and vertex array
	//vao: The vertex array to be used to draw
	//shader: The shader to be used to draw
	//count: The number of vertices to draw
	//offset: The offset in the vao to draw
	 void Draw(const VertexArray &vao, const Shader &shader, unsigned int count, unsigned int offset);
	 void ResizeWindow(int width, int height);
	//Sets the number of anit aliasing samples
	//n: The number of samples for anti aliasing
	 void SetAntiAliasingSamples(unsigned int n);
	 void EnableAntiAliasing();
	 void DisableAntiAliasing();
	//Tests related functions
	 void EnableDepthTesting();
	 void DisableDepthTesting();
	 void EnableStencilTesting();
	 void DisableStencilTesting();
	 void SetDepthMask(bool mask);
	 void SetDepthFunc(ComparisonFunc fn);
	 void SetStencilMask(unsigned int mask);
	 void SetStencilFunc(ComparisonFunc fn, int ref, unsigned int mask);
	 void SetStencilOp(TestOption stencilFail, TestOption depthFail, TestOption stencilDepthPass);
	 //Framebuffers related functions
	 void BlitNamedFrameBuffer(const FrameBuffer &fboRead, const FrameBuffer &fboDraw,
		 unsigned int srcX0, unsigned int srcY0, unsigned int srcX1, unsigned int srcY1,
		 unsigned int dstX0, unsigned int dstY0, unsigned int dstX1, unsigned int dstY1);
private:
	 GLFWwindow *m_ActiveWindow;
	 glm::vec4 s_ClearColor;
	 bool s_ClearColorBufferEnabled;
	 bool s_ClearDepthBufferEnabled;
	 bool s_ClearStencilBufferEnabled;
	 bool s_AntiAliasingEnabled;
	 unsigned int s_MultiSamples;
};

#endif // !RENDERER_H
