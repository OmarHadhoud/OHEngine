#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

#include <glad/glad.h>

#include "renderer/enums.h"

class Texture
{
public:
	Texture();
	Texture(const char * path);
	~Texture();
	unsigned int GetId() const;
	TextureType GetType() const;
	//Creates the texture with the passed screen size for the passed buffer type
	void CreateTexImage(float width, float height, BufferType bType) const;
	//Binds the 2D texture to this texture
	void Bind() const;
	void SetWrap(WrapDir dir, WrapType type) const;
	void SetMinFilter(TextureFilter filter) const;
	void SetMagFilter(TextureFilter filter) const;
	void SetType(TextureType type);
	void SetMultiSamples(unsigned int samples);
	//Unbinds the current 2D texture
	void Unbind();
	//Activates the texture with number passed
	//num: The number of texture to be active (ex: 0 = GL_TEXTURE0)
	static void Activate(unsigned int num);
	//Loads the images passed to the cubemap
	void LoadCubemapImages(std::vector<std::string> images);
	bool IsMultiSampled() const;
private:
	unsigned int m_ID;
	unsigned int m_NumSamples;
	GLenum m_Format;
	TextureType m_Type;
	void CreateTexImageNonSampled(float width, float height, BufferType bType) const;
	void CreateTexImageSampled(float width, float height, BufferType bType) const;
};

#endif // !TEXTURE_H


