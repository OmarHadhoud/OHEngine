#include <glad/glad.h>

#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "Texture.h"


Texture::Texture():m_Type(k2D), m_Path(""), m_ID(0), m_IsGammaCorrected(false)
{
	//Generate the texture
	GlCall(glGenTextures(1, &m_ID));
}

Texture::Texture(const char *path, bool gamma_corrected):m_Type(k2D), m_Path(path), m_ID(0), m_IsGammaCorrected(gamma_corrected)
{
	//Generate the texture
	GlCall(glGenTextures(1, &m_ID));
	CreateTextureFromPath();
}


Texture::~Texture()
{
	GlCall(glDeleteTextures(1, &m_ID));
}

Texture::Texture(const Texture & other)
{
	//Generate the texture
	GlCall(glGenTextures(1, &m_ID));
	//Copy the texture path
	m_Path = other.m_Path;
	//Copy the type
	m_Type = other.m_Type;
	m_IsGammaCorrected = other.m_IsGammaCorrected;
	CreateTextureFromPath();
}

Texture::Texture(Texture && other) noexcept
{
	//Move everything to the new texture
	m_ID = other.m_ID;
	m_Path = other.m_Path;
	m_Type = other.m_Type;
	m_Format = other.m_Format;
	m_InternalFormat = other.m_InternalFormat;
	m_IsGammaCorrected = other.m_IsGammaCorrected;

	//Clean the moved texture
	other.m_ID = 0;
	other.m_Path = "";
	other.m_Format = kRGB;	
}

Texture & Texture::operator=(const Texture & other)
{
	if (this != &other)
	{
		//Copy the texture path
		m_Path = other.m_Path;
		//Copy the type
		m_Type = other.m_Type;
		m_IsGammaCorrected = other.m_IsGammaCorrected;
		CreateTextureFromPath();
	}

	return *this;
}

Texture & Texture::operator=(Texture && other) noexcept
{
	//Delete the texture we currently have
	GlCall(glDeleteTextures(1, &m_ID));
	//Move everything to the new texture
	m_ID = other.m_ID;
	m_Path = other.m_Path;
	m_Type = other.m_Type;
	m_Format = other.m_Format;
	m_InternalFormat = other.m_InternalFormat;
	m_IsGammaCorrected = other.m_IsGammaCorrected;

	//Clean the moved texture
	other.m_ID = 0;
	other.m_Path = "";
	other.m_Format = kRGB;

	return *this;
}

unsigned int Texture::GetId() const
{
	return m_ID;
}

std::string Texture::GetPath() const
{
	return m_Path;
}

TextureFormat Texture::GetFormat() const
{
	return m_Format;
}

TextureFormat Texture::GetInternalFormat() const
{
	return m_InternalFormat;
}

void Texture::SetInternalFormat(TextureFormat format)
{
	m_InternalFormat = format;
}

TextureType Texture::GetType() const
{
	return m_Type;
}

void Texture::CreateTextureFromPath()
{
	if (m_Path == "")
		return;
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
	unsigned char *data = stbi_load(m_Path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 3)
		{
			SetInternalFormat(m_IsGammaCorrected == true ? kSRGB : kRGB);
			SetFormat(kRGB);
		}
		else if (nrChannels == 4)
		{
			SetInternalFormat(m_IsGammaCorrected == true ? kSRGBA :  kRGBA);
			SetFormat(kRGBA);
		}
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_Format, GL_UNSIGNED_BYTE, data));
		GlCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::CreateTexImage(float width, float height, BufferType bType) const
{
	if (bType == kColor)
	{
		GlCall(glTexImage2D(m_Type, 0, GL_RGB16, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}
	else if (bType == kDepth)
	{
		GlCall(glTexImage2D(m_Type, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
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

void Texture::Bind() const
{	
	GlCall(glBindTexture(m_Type, m_ID));
}

void Texture::SetWrap(WrapDir dir, WrapType type) const
{
	//Set the texture wrapping Settings
	GlCall(glTexParameteri(m_Type, dir, type));
}

void Texture::SetBorderColor(float* color) const
{
	GlCall(glTexParameterfv(m_Type,GL_TEXTURE_BORDER_COLOR, color));
}

void Texture::SetMinFilter(TextureFilter filter) const
{	
	//Set the texture filtering Settings
	GlCall(glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, filter));
}

void Texture::SetMagFilter(TextureFilter filter) const
{
	//Set the texture filtering Settings
	GlCall(glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, filter));
}

void Texture::SetType(TextureType type)
{
	m_Type = type;
}
void Texture::SetFormat(TextureFormat format)
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

void Texture::SetGammaCorrection(bool val)
{
	m_IsGammaCorrected = val;
}
