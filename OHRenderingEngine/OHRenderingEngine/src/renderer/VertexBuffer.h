#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

#include "renderer/enums.h"
#include "renderer/VertexBufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(unsigned int size, GLenum usage);
	VertexBuffer(void* data, unsigned int size, GLenum usage);
	~VertexBuffer();
	//Copy constructor
	VertexBuffer(const VertexBuffer& other) = delete;
	//Move constructor
	VertexBuffer(VertexBuffer && other) noexcept;
	//Copy assignment operator
	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	//Move assignment operator
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	//Binds the current vertex buffer to this
	void Bind() const;
	//Unbinds the vertex buffer
	static void Unbind();
	//Buffers the data for the buffe
	void BufferData(void* data, unsigned int size, GLenum usage);
	//Buffers a part of the data
	//data: The data to be copied to GPU
	//size: The size of the data in bytes
	//offset: The offset of the buffer data in bytes
	void BufferSubData(void* data, unsigned int size, unsigned int offset) const;
	const VertexBufferLayout& GetLayout() const;
	void SetLayout(VertexBufferLayout &layout);
private:
	unsigned int m_ID;
	VertexBufferLayout m_Layout;
};

#endif	//VERTEX_BUFFER_H

