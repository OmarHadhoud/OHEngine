
#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "MultiSampledTexture.h"

MultiSampledTexture::MultiSampledTexture()
{
	SetType(k2DMS);
}


MultiSampledTexture::~MultiSampledTexture()
{
}

void MultiSampledTexture::CreateTexImage(float width, float height, BufferType bType) const
{
	if (bType == kColor)
	{
		GlCall(glTexImage2DMultisample(GetType() , m_NumSamples, GL_RGB16, width, height, GL_TRUE));
	}
	else if (bType == kDepth)
	{
		GlCall(glTexImage2DMultisample(GetType(), m_NumSamples, GL_DEPTH_COMPONENT, width, height, GL_TRUE));
	}
	else if (bType == kStencil)
	{
		GlCall(glTexImage2DMultisample(GetType(), m_NumSamples, GL_STENCIL_INDEX, width, height, GL_TRUE));
	}
	else if (bType == kDepthStencil)
	{
		GlCall(glTexImage2DMultisample(GetType(), m_NumSamples, GL_DEPTH24_STENCIL8, width, height, GL_TRUE));
	}
}

void MultiSampledTexture::SetMultiSamples(unsigned int samples)
{
	m_NumSamples = samples;
}
