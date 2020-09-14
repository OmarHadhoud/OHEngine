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
	static void Clear(unsigned int flag, glm::vec4 color);
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
	void SetAntiAliasingSamples(unsigned int n);
	void EnableAntiAliasing();
	void DisableAntiAliasing();
	//Tests, Masks, Features related functions
	static void EnableDepthTesting() ;
	static void DisableDepthTesting() ;
	static void EnableStencilTesting() ;
	static void DisableStencilTesting() ;
	static void EnableBlending();
	static void DisableBlending();
	static void EnableCulling();
	static void DisableCulling();
	static void SetDepthMask(bool mask) ;
	static void SetDepthFunc(ComparisonFunc fn) ;
	static void SetStencilMask(unsigned int mask) ;
	static void SetStencilFunc(ComparisonFunc fn, int ref, unsigned int mask) ;
	static void SetStencilOp(TestOption stencilFail, TestOption depthFail, TestOption stencilDepthPass) ;
	//Framebuffers related functions
	static void BlitNamedFrameBuffer(const FrameBuffer &fboRead, const FrameBuffer &fboDraw,
		unsigned int srcX0, unsigned int srcY0, unsigned int srcX1, unsigned int srcY1,
		unsigned int dstX0, unsigned int dstY0, unsigned int dstX1, unsigned int dstY1) ;
private:
	GLFWwindow *m_ActiveWindow;
	bool s_AntiAliasingEnabled;
	unsigned int s_MultiSamples;
};

#endif // !RENDERER_H
