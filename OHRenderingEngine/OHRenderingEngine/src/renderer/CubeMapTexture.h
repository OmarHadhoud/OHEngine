#ifndef CUBE_MAP_TEXTURE_H
#define CUBE_MAP_TEXTURE_H

#include "renderer/Texture.h"

class CubeMapTexture : public Texture
{
public:
	CubeMapTexture(std::vector<std::string> images);
	virtual ~CubeMapTexture();
private:

};

#endif //!CUBE_MAP_TEXTURE_H