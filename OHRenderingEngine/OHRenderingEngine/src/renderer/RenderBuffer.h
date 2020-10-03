#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "renderer/enums.h"

class RenderBuffer
{
public:
	RenderBuffer();
	~RenderBuffer();
	unsigned int GetId() const;
	//Binds the current render buffer to this
	void Bind() const;
	//Unbinds the render buffer
	void Unbind() const;
	//Creates the render buffer object
	void Create(float width, float height, GLenum bType) const;
	void EnableMultiSampled();
	void DisableMultiSampled();
	void SetMultiSamples(unsigned int samples);
	bool IsMultiSampled() const;
private:
	unsigned int m_ID;
	unsigned int m_NumSamples;
	bool m_MultiSampledEnabled;
	void CreateMultiSampled(float width, float height, GLenum bType) const;
	void CreateNonSampled(float width, float height, GLenum bType) const;
};

#endif	//RENDER_BUFFER_H

