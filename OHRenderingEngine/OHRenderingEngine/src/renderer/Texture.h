#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:
	Texture(const char * path);
	~Texture();
	unsigned int GetId() const;
	void Bind() const;
	static void Activate(unsigned int num);
private:
	unsigned int m_ID;
	GLenum m_Format;
};

#endif // !TEXTURE_H


