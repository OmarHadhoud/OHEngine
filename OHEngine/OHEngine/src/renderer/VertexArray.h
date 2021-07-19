#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	//Copy constructor
	VertexArray(const VertexArray& other) = delete;
	//Move constructor
	VertexArray(VertexArray && other) noexcept;
	//Copy assignment operator
	VertexArray& operator=(const VertexArray& other) = delete;
	//Move assignment operator
	VertexArray& operator=(VertexArray&& other) noexcept;
	//Binds the current vertex array to this one
	void Bind() const;
	//Unbinds the current vertex array
	static void Unbind();
	//Adds a vertex buffer to this vertex array
	//vb: The vertex buffer to be added
	void AddBuffer(const VertexBuffer &vb);
private:
	unsigned int m_ID;
	unsigned int m_BoundedAttributes;
};

#endif	//VERTEX_ARRAY_H

