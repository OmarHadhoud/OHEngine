#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

#include "renderer/enums.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(unsigned int size, BUFFER_USAGE usage);
	VertexBuffer(void *data, unsigned int size, BUFFER_USAGE usage);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
	void BufferSubData(void *data, unsigned int size, unsigned int offset) const;
private:
	unsigned int m_ID;
};

#endif	//VERTEX_BUFFER_H

