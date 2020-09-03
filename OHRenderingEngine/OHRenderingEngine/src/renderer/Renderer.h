#ifndef RENDERER_H
#define RENDERER_H

#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/Texture.h"
#include "renderer/Debugger.h"


class Renderer
{
public:
	Renderer();
	~Renderer();
	static void ClearScreen(float r, float g, float b, float a);
	static void Draw(const VertexArray &vao, const Shader &shader, unsigned int count, unsigned int offset);
	static void ResizeWindow(int width, int height);
};

#endif // !RENDERER_H
