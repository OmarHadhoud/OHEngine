#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include "renderer/enums.h"

class Texture
{
public:
	Texture(unsigned int activeTex = 0);
	Texture(const char * path);
	~Texture();
	unsigned int GetId() const;
	//Creates the texture with the passed screen size for the passed buffer type
	void CreateTexImage(float width, float height, BufferType bType) const;
	//Binds the 2D texture to this texture
	void Bind() const;
	void SetWrap(WrapDir dir, WrapType type) const;
	void SetMinFilter(TextureFilter filter) const;
	void SetMagFilter(TextureFilter filter) const;
	//Unbinds the current 2D texture
	static void Unbind();
	//Activates the texture with number passed
	//num: The number of texture to be active (ex: 0 = GL_TEXTURE0)
	static void Activate(unsigned int num);
private:
	unsigned int m_ID;
	GLenum m_Format;
};

#endif // !TEXTURE_H


