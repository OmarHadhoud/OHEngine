#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <vector>

#include <glad/glad.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
	{
		m_stride_ = 0;
	}
	std::vector<VertexBufferElement> GetElements() const 
	{
		return m_elements_;
	}
	unsigned int GetStride() const
	{
		return m_stride_;
	}
	//Note: Implemented here as emplicit specialization for template is not allowed in cpp files.
	template<typename T>
	void Push(unsigned int count, bool normalized)
	{
		static_assert(false);
	}
	template<> void Push<int>(unsigned int count, bool normalized)
	{
		VertexBufferElement e;
		e.count = count;
		e.type = GL_INT;
		e.normalized = normalized;
		m_elements_.push_back(e);
		m_stride_ += count * sizeof(int);
	}
	template<> void Push<float>(unsigned int count, bool normalized)
	{
		VertexBufferElement e;
		e.count = count;
		e.type = GL_FLOAT;
		e.normalized = normalized;
		m_elements_.push_back(e);
		m_stride_ += count * sizeof(float);
	}
	template<> void Push<unsigned int>(unsigned int count, bool normalized)
	{
		VertexBufferElement e;
		e.count = count;
		e.type = GL_UNSIGNED_INT;
		e.normalized = normalized;
		m_elements_.push_back(e);
		m_stride_ += count * sizeof(unsigned int);
	}
private:
	std::vector<VertexBufferElement> m_elements_;
	unsigned int m_stride_;
};

#endif	//VERTEX_BUFFER_LAYOUT_H
