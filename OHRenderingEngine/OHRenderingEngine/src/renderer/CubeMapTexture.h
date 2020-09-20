#ifndef CUBE_MAP_TEXTURE_H
#define CUBE_MAP_TEXTURE_H

#include "renderer/Texture.h"

class CubeMapTexture : public Texture
{
public:
	CubeMapTexture();
	CubeMapTexture(std::vector<std::string> images);
	virtual void CreateTexImage(float width, float height, BufferType bType) const;
	virtual ~CubeMapTexture();
private:

};

#endif //!CUBE_MAP_TEXTURE_H