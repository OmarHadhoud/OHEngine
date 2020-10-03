
#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture()
{
	SetType(GL_TEXTURE_CUBE_MAP);
}

CubeMapTexture::CubeMapTexture(std::vector<std::string> images)
{
	SetType(GL_TEXTURE_CUBE_MAP);
	Bind();
	//Load and generate the texture
	int width, height, nrChannels;
	for (int i = 0; i < images.size(); i++)
	{
		unsigned char *data = stbi_load(images[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			if (nrChannels == 3)
			{
				SetFormat(GL_RGB);
				SetInternalFormat(GL_SRGB);
			}
			else if (nrChannels == 4)
			{
				SetFormat(GL_RGBA);
				SetInternalFormat(GL_SRGB_ALPHA);
			}
			GLenum format = GetFormat();
			GLenum internalFormat = GetInternalFormat();
			GlCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture at " << images[i].c_str() << " !" << std::endl;
			stbi_image_free(data);
		}
	}
	Unbind();
}

void CubeMapTexture::CreateTexImage(float width, float height, GLenum bType) const
{
	Bind();
	
	//Load and generate the texture
	for (int i = 0; i < 6; i++)
	{
		if (bType == GL_RGB)
		{
			GlCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
		}
		else if (bType == GL_DEPTH_COMPONENT)
		{
			GlCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
		}
		else if (bType == GL_STENCIL_ATTACHMENT)
		{
			GlCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_ATTACHMENT, GL_UNSIGNED_BYTE, nullptr));
		}
		else if (bType == GL_DEPTH_STENCIL_ATTACHMENT)
		{
			GlCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL_ATTACHMENT, GL_UNSIGNED_INT_24_8, nullptr));
		}
	}
	Unbind();
}


CubeMapTexture::~CubeMapTexture()
{
}
