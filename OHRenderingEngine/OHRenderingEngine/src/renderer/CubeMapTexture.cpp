
#include "dependencies/utils/stb_image.h"
#include "renderer/Debugger.h"
#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture(std::vector<std::string> images)
{
	SetType(kCubeMap);
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
				SetFormat(kRGB);
				SetInternalFormat(kSRGB);
			}
			else if (nrChannels == 4)
			{
				SetFormat(kRGBA);
				SetInternalFormat(kSRGBA);
			}
			TextureFormat format = GetFormat();
			TextureFormat internalFormat = GetInternalFormat();
			GlCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, internalFormat , width, height, 0, format, GL_UNSIGNED_BYTE, data));
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


CubeMapTexture::~CubeMapTexture()
{
}
