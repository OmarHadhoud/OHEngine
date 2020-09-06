#include <glad/glad.h>

#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "Texture.h"


Texture::Texture(unsigned int activeTex)
{
	//Generate the texture and bind it
	GlCall(glGenTextures(1, &m_ID));
	if (activeTex == -1) //Don't initialize yet
		return;
	//Activate before binding
	Activate(activeTex);
	Bind();
	//If not specefied, wrap with repeat and linear filters
	SetWrap(kS, kRepeat);
	SetWrap(kT, kRepeat);
	SetMinFilter(kLinear);
	SetMagFilter(kLinear);
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

void Texture::CreateTexImage(float width, float height, BufferType bType) const
{
	Bind();
	if (bType == kColor)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}
	else if (bType == kDepth)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_ATTACHMENT, GL_UNSIGNED_BYTE, NULL));
	}
	else if (bType == kStencil)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_ATTACHMENT, GL_UNSIGNED_BYTE, NULL));
	}
	else if (bType == kDepthStencil)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL_ATTACHMENT, GL_UNSIGNED_INT_24_8, NULL));
	}
}

void Texture::Bind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::SetWrap(WrapDir dir, WrapType type) const
{
	Bind();
	//Set the texture wrapping Settings
	GlCall(glTexParameteri(GL_TEXTURE_2D, dir, type));
	GlCall(glTexParameteri(GL_TEXTURE_2D, dir, type));
}

void Texture::SetMinFilter(TextureFilter filter) const
{	
	Bind();
	//Set the texture filtering Settings
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
}

void Texture::SetMagFilter(TextureFilter filter) const
{
	Bind();
	//Set the texture filtering Settings
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
}

void Texture::Unbind()
{
	GlCall(glBindTexture(GL_TEXTURE_2D,0));

}

void Texture::Activate(unsigned int num)
{
	GlCall(glActiveTexture(GL_TEXTURE0 + num));
}
