#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:
	Texture(const char * path);
	~Texture();
	unsigned int GetId() const;
	//Binds the 2D texture to this texture
	void Bind() const;
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


