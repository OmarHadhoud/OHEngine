#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <src/dependencies/utils/stb_image.h>

class Shader
{
public:
	//Shader constructor that reads the shader files and compile it
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	//To use the shader program
	void Use() const;
	unsigned int GetID() const;
	//Utility uniform functions
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetVec2(const std::string &name, glm::vec2 value) const;
	void SetVec3(const std::string &name, glm::vec3 value) const;
	void SetVec4(const std::string &name, glm::vec4 value) const;
	void SetMat2(const std::string &name, glm::mat2 value) const;
	void SetMat3(const std::string &name, glm::mat3 value) const;
	void SetMat4(const std::string &name, glm::mat4 value) const;
private:
	//The shader program ID
	unsigned int m_ID;
};

#endif	//SHADER_H
