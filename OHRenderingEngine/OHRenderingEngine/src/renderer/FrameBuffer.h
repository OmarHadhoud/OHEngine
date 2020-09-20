#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "renderer/Texture.h"
#include "renderer/RenderBuffer.h"
#include "renderer/enums.h"

class FrameBuffer
{
public:
	FrameBuffer(FrameBufferTarget target = kReadDraw);
	~FrameBuffer();
	unsigned int GetID() const;
	//Binds the current frame buffer to this
	void Bind() const;
	void Bind(FrameBufferTarget target) const;
	//Unbinds the frame buffer
	void Unbind() const;
	//Attaches texture object to frame buffer
	void AttachTexture(Texture &tex, FrameBufferAttachement attach, bool is_cubemap = false) const;
	//Attaches render object to frame buffer
	void AttachRenderObject(RenderBuffer &rbo, FrameBufferAttachement attach) const;
	//Checks if the frame buffer is complete
	bool IsComplete() const;
	//Sets the read and draw buffer
	void DrawBuffer(BufferType buffer);
	void ReadBuffer(BufferType buffer);
private:
	unsigned int m_ID;
	FrameBufferTarget m_Target;
};

#endif	//FRAME_BUFFER_H

