#include <glad/glad.h>

#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "Texture.h"


Texture::Texture(const char *path)
{
	//Generate the texture and bind it
	GlCall(glGenTextures(1, &m_id_));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_id_));
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
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data));
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
	return m_id_;
}

void Texture::Bind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D,m_id_));
}

void Texture::Activate(unsigned int num)
{
	GlCall(glActiveTexture(GL_TEXTURE0 + num));
}