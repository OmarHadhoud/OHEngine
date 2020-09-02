#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddBuffer(VertexBuffer &vb, VertexBufferLayout &vbl);
private:
	unsigned int m_id_;
};

#endif	//VERTEX_ARRAY_H

