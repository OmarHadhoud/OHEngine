#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "renderer/enums.h"

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(unsigned int size, BUFFER_USAGE usage);
	IndexBuffer(void *data, unsigned int size, BUFFER_USAGE usage);
	~IndexBuffer();
	//Binds the current index buffer to this
	void Bind() const;
	//Unbinds the index buffer
	void Unbind() const;
	//Buffers a part of the data
	//data: The data to be copied to GPU
	//size: The size of the data in bytes
	//offset: The offset of the buffer data in bytes
	void BufferSubData(void *data, unsigned int size, unsigned int offset) const;
private:
	unsigned int m_ID;
};

#endif	//INDEX_BUFFER_H

