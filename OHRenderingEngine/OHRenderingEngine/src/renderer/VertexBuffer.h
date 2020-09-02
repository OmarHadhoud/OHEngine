#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(unsigned int size, GLenum usage);
	VertexBuffer(void *data, unsigned int size, GLenum usage);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
	void BufferSubData(void *data, unsigned int size, unsigned int offset) const;
private:
	unsigned int m_id_;
};

#endif	//VERTEX_BUFFER_H

