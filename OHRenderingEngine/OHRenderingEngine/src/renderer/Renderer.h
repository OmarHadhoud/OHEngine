#ifndef RENDERER_H
#define RENDERER_H


#include "renderer/FrameBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/Shader.h"
#include "renderer/Camera.h"
#include "renderer/Debugger.h"


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
	static void SetLineWidth(float width);
	static void DrawPoints(int count);
	//Sets the number of anit aliasing samples
	//n: The number of samples for anti aliasing
	void SetAntiAliasingSamples(unsigned int n);
	static void EnableAntiAliasing();
	void DisableAntiAliasing();
	//Tests related functions
	static void EnableDepthTesting() ;
	static void DisableDepthTesting() ;
	static void EnableStencilTesting() ;
	static void DisableStencilTesting() ;
	static void SetDepthMask(bool mask) ;
	static void SetDepthFunc(GLenum fn) ;
	static void SetStencilMask(unsigned int mask) ;
	static void SetStencilFunc(GLenum fn, int ref, unsigned int mask) ;
	static void SetStencilOp(GLenum stencilFail, GLenum depthFail, GLenum stencilDepthPass) ;
	//Blending related functions
	static void EnableBlending();
	static void DisableBlending();
	static void SetBlendFactors(GLenum source_factor, GLenum dest_factor);
	static void SetBlendFactorsSeparate(GLenum source_factor_rgb, GLenum dest_factor_rgb, GLenum source_factor_a, GLenum dest_factor_a);
	static void SetBlendEquation(GLenum func);
	//Culling related functions
	static void EnableCulling();
	static void DisableCulling();
	static void CullFace(GLenum face);
	static void SetFrontFace(GLenum dir);
	//Gamma correction functions
	static void EnableGammaCorrection();
	static void DisableGammaCorrection();
	//Framebuffers related functions
	static void BlitNamedFrameBuffer(const FrameBuffer &fboRead, const FrameBuffer &fboDraw,
		unsigned int srcX0, unsigned int srcY0, unsigned int srcX1, unsigned int srcY1,
		unsigned int dstX0, unsigned int dstY0, unsigned int dstX1, unsigned int dstY1) ;
	//Vertex Array related functions
	static void VertexAttribPointer(unsigned int index, int size, GLenum type, bool normalized, GLsizei stride, const void* pointer);
	static void EnableVertexAttribArray(int index);
private:
	GLFWwindow *m_ActiveWindow;
	static bool s_AntiAliasingEnabled;
};

#endif // !RENDERER_H
