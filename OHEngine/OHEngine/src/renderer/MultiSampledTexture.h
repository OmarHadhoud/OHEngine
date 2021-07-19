#ifndef MULTI_SAMPLED_TEXTURE_H
#define MULTI_SAMPLED_TEXTURE_H

#include "renderer/Texture.h"

class MultiSampledTexture : public Texture
{
public:
	MultiSampledTexture();
	virtual ~MultiSampledTexture();
	//Creates the texture with the passed screen size for the passed buffer type
	virtual void CreateTexImage(float width, float height, GLenum bType) const;
	void SetMultiSamples(unsigned int samples);
private:
	unsigned int m_NumSamples;
};

#endif //!MULTI_SAMPLED_TEXTURE_H