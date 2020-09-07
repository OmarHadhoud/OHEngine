#include <glad/glad.h>

#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "Texture.h"


Texture::Texture(): m_NumSamples(1), m_Type(k2D)
{
	//Generate the texture
	GlCall(glGenTextures(1, &m_ID));
}

Texture::Texture(const char *path)
{
	//Generate the texture and bind it
	GlCall(glGenTextures(1, &m_ID));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_ID));
	//Set the texture wrapping Settings
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	//Set the texture filtering Settings
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//Load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 1)
			m_Format = GL_RED;
		else if (nrChannels == 3)
			m_Format = GL_RGB;
		else if (nrChannels == 4)
			m_Format = GL_RGBA;
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, m_Format, width, height, 0, m_Format, GL_UNSIGNED_BYTE, data));
		GlCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


Texture::~Texture()
{
}

unsigned int Texture::GetId() const
{
	return m_ID;
}

TextureType Texture::GetType() const
{
	return m_Type;
}

void Texture::CreateTexImage(float width, float height, BufferType bType) const
{
	Bind();
	if (m_Type == k2DMS)
		CreateTexImageSampled(width, height, bType);
	else
		CreateTexImageNonSampled(width, height, bType);
}

void Texture::Bind() const
{	
	GlCall(glBindTexture(m_Type, m_ID));
}

void Texture::SetWrap(WrapDir dir, WrapType type) const
{
	Bind();
	//Set the texture wrapping Settings
	GlCall(glTexParameteri(m_Type, dir, type));
}

void Texture::SetMinFilter(TextureFilter filter) const
{	
	Bind();
	//Set the texture filtering Settings
	GlCall(glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, filter));
}

void Texture::SetMagFilter(TextureFilter filter) const
{
	Bind();
	//Set the texture filtering Settings
	GlCall(glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, filter));
}

void Texture::SetType(TextureType type)
{
	m_Type = type;
}

void Texture::SetMultiSamples(unsigned int samples)
{
	m_NumSamples = samples;
}


void Texture::Unbind()
{
	GlCall(glBindTexture(m_Type,0));

}

void Texture::Activate(unsigned int num)
{
	GlCall(glActiveTexture(GL_TEXTURE0 + num));
}

void Texture::LoadCubemapImages(std::vector<std::string> images)
{
	Bind();
	//Load and generate the texture
	int width, height, nrChannels;
	for (int i = 0; i < images.size(); i++)
	{
		unsigned char *data = stbi_load(images[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			if (nrChannels == 1)
				m_Format = GL_RED;
			else if (nrChannels == 3)
				m_Format = GL_RGB;
			else if (nrChannels == 4)
				m_Format = GL_RGBA;
			GlCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, m_Format, width, height, 0, m_Format, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture at " << images[i].c_str() << " !" << std::endl;
			stbi_image_free(data);
		}

	}
}

bool Texture::IsMultiSampled() const
{
	return m_Type==k2DMS;
}

void Texture::CreateTexImageNonSampled(float width, float height, BufferType bType) const
{
	if (bType == kColor)
	{
		GlCall(glTexImage2D(m_Type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}
	else if (bType == kDepth)
	{
		GlCall(glTexImage2D(m_Type, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_ATTACHMENT, GL_UNSIGNED_BYTE, NULL));
	}
	else if (bType == kStencil)
	{
		GlCall(glTexImage2D(m_Type, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_ATTACHMENT, GL_UNSIGNED_BYTE, NULL));
	}
	else if (bType == kDepthStencil)
	{
		GlCall(glTexImage2D(m_Type, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL_ATTACHMENT, GL_UNSIGNED_INT_24_8, NULL));
	}
}

void Texture::CreateTexImageSampled(float width, float height, BufferType bType) const
{
	if (bType == kColor)
	{
		GlCall(glTexImage2DMultisample(m_Type, m_NumSamples, GL_RGB, width, height, GL_TRUE));
	}
	else if (bType == kDepth)
	{
		GlCall(glTexImage2DMultisample(m_Type, m_NumSamples, GL_DEPTH_COMPONENT, width, height, GL_TRUE));
	}																						    
	else if (bType == kStencil)																    
	{																						    
		GlCall(glTexImage2DMultisample(m_Type, m_NumSamples, GL_STENCIL_INDEX, width, height, GL_TRUE));
	}										   												   
	else if (bType == kDepthStencil)		   												   
	{										 												   
		GlCall(glTexImage2DMultisample(m_Type, m_NumSamples, GL_DEPTH24_STENCIL8, width, height, GL_TRUE));
	}
}
