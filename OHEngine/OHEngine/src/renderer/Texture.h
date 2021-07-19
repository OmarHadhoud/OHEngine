#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>


class Texture
{
public:
	Texture();
	Texture(const char * path, bool gamma_corrected = false);
	virtual ~Texture();
	//Copy constructor
	Texture(const Texture& other);					
	//Move constructor
	Texture(Texture && other) noexcept;
	//Copy assignment operator
	Texture& operator=(const Texture& other);		
	//Move assignment operator
	Texture& operator=(Texture&& other) noexcept;
	unsigned int GetId() const;
	std::string GetPath() const;
	virtual GLenum GetType() const;
	//Loads the image texture
	void CreateTextureFromPath();
	//Creates the texture with the passed screen size for the passed buffer type
	virtual void CreateTexImage(float width, float height, GLenum bType) const;
	//Binds the 2D texture to this texture
	void Bind() const;
	void SetWrap(GLenum dir, GLenum type) const;
	void SetBorderColor(float* color) const;
	void SetMinFilter(GLenum filter) const;
	void SetMagFilter(GLenum filter) const;
	void SetType(GLenum type);
	//Unbinds the current 2D texture
	void Unbind() const;
	//Activates the texture with number passed
	//num: The number of texture to be active (ex: 0 = GL_TEXTURE0)
	static void Activate(unsigned int num);
	bool IsMultiSampled() const;
	//Gamma correction functions
	void SetGammaCorrection(bool val);
private:
	unsigned int m_ID;
	bool m_IsGammaCorrected;
	GLenum m_Format;
	GLenum m_InternalFormat;
	GLenum m_Type;
	std::string m_Path;
protected:
	GLenum GetFormat() const;
	GLenum GetInternalFormat() const;
	void SetFormat(GLenum format);
	void SetInternalFormat(GLenum format);
};

#endif // !TEXTURE_H


