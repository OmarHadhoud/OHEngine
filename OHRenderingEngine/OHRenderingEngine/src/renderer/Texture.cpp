#include <glad/glad.h>

#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "Texture.h"


Texture::Texture():m_Type(k2D), m_Path("")
{
	//Generate the texture
	GlCall(glGenTextures(1, &m_ID));
}

Texture::Texture(const char *path): m_Path(path)
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
			SetFormat(GL_RED);
		else if (nrChannels == 3)
			SetFormat(GL_RGB);
		else if (nrChannels == 4)
			SetFormat(GL_RGBA);
		GLenum format = GetFormat();
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
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
	GlCall(glDeleteTextures(1, &m_ID));
}

unsigned int Texture::GetId() const
{
	return m_ID;
}

std::string Texture::GetPath() const
{
	return m_Path;
}

GLenum Texture::GetFormat() const
{
	return m_Format;
}

TextureType Texture::GetType() const
{
	return m_Type;
}

void Texture::CreateTexImage(float width, float height, BufferType bType) const
{
	Bind();
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
	Unbind();
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
	Unbind();
}

void Texture::SetMinFilter(TextureFilter filter) const
{	
	Bind();
	//Set the texture filtering Settings
	GlCall(glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, filter));
	Unbind();
}

void Texture::SetMagFilter(TextureFilter filter) const
{
	Bind();
	//Set the texture filtering Settings
	GlCall(glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, filter));
	Unbind();
}

void Texture::SetType(TextureType type)
{
	m_Type = type;
}
void Texture::SetFormat(GLenum format)
{
	m_Format = format;
}

void Texture::Unbind() const
{
	GlCall(glBindTexture(m_Type,0));
}

void Texture::Activate(unsigned int num)
{
	GlCall(glActiveTexture(GL_TEXTURE0 + num));
}



bool Texture::IsMultiSampled() const
{
	return m_Type==k2DMS;
}
