#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

#include "renderer/enums.h"
#include "renderer/VertexBufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(BUFFER_USAGE usage);
	VertexBuffer(unsigned int size, BUFFER_USAGE usage);
	VertexBuffer(void* data, unsigned int size, BUFFER_USAGE usage);
	~VertexBuffer();
	//Binds the current vertex buffer to this
	void Bind() const;
	//Unbinds the vertex buffer
	void Unbind() const;
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

