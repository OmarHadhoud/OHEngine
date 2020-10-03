
#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "MultiSampledTexture.h"

MultiSampledTexture::MultiSampledTexture()
{
	SetType(GL_TEXTURE_2D_MULTISAMPLE);
}


MultiSampledTexture::~MultiSampledTexture()
{
}

void MultiSampledTexture::CreateTexImage(float width, float height, GLenum bType) const
{
	if (bType == GL_RGB)
	{
		GlCall(glTexImage2DMultisample(GetType() , m_NumSamples, GL_RGB16, width, height, GL_TRUE));
	}
	else if (bType == GL_RGB16F)
	{
		GlCall(glTexImage2DMultisample(GetType(), m_NumSamples, GL_RGB16F, width, height, GL_TRUE));
	}
	else if (bType == GL_DEPTH_COMPONENT)
	{
		GlCall(glTexImage2DMultisample(GetType(), m_NumSamples, GL_DEPTH_COMPONENT, width, height, GL_TRUE));
	}
	else if (bType == GL_STENCIL_ATTACHMENT)
	{
		GlCall(glTexImage2DMultisample(GetType(), m_NumSamples, GL_STENCIL_INDEX, width, height, GL_TRUE));
	}
	else if (bType == GL_DEPTH_STENCIL_ATTACHMENT)
	{
		GlCall(glTexImage2DMultisample(GetType(), m_NumSamples, GL_DEPTH24_STENCIL8, width, height, GL_TRUE));
	}
}

void MultiSampledTexture::SetMultiSamples(unsigned int samples)
{
	m_NumSamples = samples;
}
