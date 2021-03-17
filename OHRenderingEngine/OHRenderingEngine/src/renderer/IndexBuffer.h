#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(unsigned int size, GLenum usage);
	IndexBuffer(void *data, unsigned int size, GLenum usage);
	~IndexBuffer();
	//Copy constructor
	IndexBuffer(const IndexBuffer& other) = delete;
	//Move constructor
	IndexBuffer(IndexBuffer && other) noexcept;
	//Copy assignment operator
	IndexBuffer& operator=(const IndexBuffer& other) = delete;
	//Move assignment operator
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	//Binds the current index buffer to this
	void Bind() const;
	//Unbinds the index buffer
	static void Unbind();
	//Buffers the data for the buffer
	void BufferData(void *data, unsigned int size, GLenum usage) const;
	//Buffers a part of the data
	//data: The data to be copied to GPU
	//size: The size of the data in bytes
	//offset: The offset of the buffer data in bytes
	void BufferSubData(void *data, unsigned int size, unsigned int offset) const;
private:
	unsigned int m_ID;
};

#endif	//INDEX_BUFFER_H

