#include <glad/glad.h>

#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "Texture.h"


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

void Texture::Bind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::Unbind()
{
	GlCall(glBindTexture(GL_TEXTURE_2D,0));

}

void Texture::Activate(unsigned int num)
{
	GlCall(glActiveTexture(GL_TEXTURE0 + num));
}
