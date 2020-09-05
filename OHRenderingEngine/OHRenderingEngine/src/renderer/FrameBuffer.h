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
	//Binds the current frame buffer to this
	void Bind() const;
	//Unbinds the frame buffer
	void Unbind() const;
	//Attaches texture object to frame buffer
	void AttachTexture(Texture &tex, FrameBufferAttachement attach) const;
	//Attaches render object to frame buffer
	void AttachRenderObject(RenderBuffer &rbo, FrameBufferAttachement attach) const;
private:
	unsigned int m_ID;
	FrameBufferTarget m_Target;
};

#endif	//FRAME_BUFFER_H

