#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(unsigned int size, GLenum usage);
	IndexBuffer(void *data, unsigned int size, GLenum usage);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	void BufferSubData(void *data, unsigned int size, unsigned int offset) const;
private:
	unsigned int m_ID;
};

#endif	//INDEX_BUFFER_H

