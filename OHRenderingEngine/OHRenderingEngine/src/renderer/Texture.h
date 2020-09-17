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
	virtual TextureType GetType() const;
	//Loads the image texture
	void CreateTextureFromPath();
	//Creates the texture with the passed screen size for the passed buffer type
	virtual void CreateTexImage(float width, float height, BufferType bType) const;
	//Binds the 2D texture to this texture
	void Bind() const;
	void SetWrap(WrapDir dir, WrapType type) const;
	void SetMinFilter(TextureFilter filter) const;
	void SetMagFilter(TextureFilter filter) const;
	void SetType(TextureType type);
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
	TextureFormat m_Format;
	TextureFormat m_InternalFormat;
	TextureType m_Type;
	std::string m_Path;
protected:
	TextureFormat GetFormat() const;
	TextureFormat GetInternalFormat() const;
	void SetFormat(TextureFormat format);
	void SetInternalFormat(TextureFormat format);
};

#endif // !TEXTURE_H


