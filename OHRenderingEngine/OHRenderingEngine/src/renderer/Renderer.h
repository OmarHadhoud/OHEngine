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
	static void SetClearColor(float r, float g, float b, float a);
	static void EnableClearColor();
	static void DisableClearColor();
	static void EnableClearDepth();
	static void DisableClearDepth();
	static void Clear();
	static void Draw(const VertexArray &vao, const Shader &shader, unsigned int count, unsigned int offset);
	static void ResizeWindow(int width, int height);
	static void EnableDepthTesting();
	static void SetAntiAliasingSamples(unsigned int n);
	static void EnableAntiAliasing();
private:
	Renderer();
	~Renderer();
	static glm::vec4 s_ClearColor;
	static bool s_ClearColorEnabled;
	static bool s_ClearDepthEnabled;
};

#endif // !RENDERER_H
