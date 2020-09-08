#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	//Binds the current vertex array to this one
	void Bind() const;
	//Unbinds the current vertex array
	void Unbind() const;
	//Adds a vertex buffer to this vertex array
	//vb: The vertex buffer to be added
	//vbl: The vertex buffer layout
	void AddBuffer(const VertexBuffer &vb);
private:
	unsigned int m_ID;
};

#endif	//VERTEX_ARRAY_H

