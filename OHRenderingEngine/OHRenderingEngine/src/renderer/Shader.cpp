#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
	//1-Retrieve the vertex and fragment source code from file path
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//read file buffer into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//close file handlers
		vShaderFile.close();
		fShaderFile.close();
		//convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//2-Compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	// shader Program
	m_id_ = glCreateProgram();
	glAttachShader(m_id_, vertex);
	glAttachShader(m_id_, fragment);
	glLinkProgram(m_id_);
	// print linking errors if any
	glGetProgramiv(m_id_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_id_, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n\t" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
}

void Shader::Use() const
{
	//Activate this shader
	glUseProgram(this->m_id_);
}

unsigned int Shader::GenerateTexture(const char * path, const char * type)
{
	//The texutre id
	unsigned int texture;
	//Generate the texture and bind it
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//Set the texture wrapping Settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Set the texture filtering Settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (type == "jpg")
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (type == "png")
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

void Shader::SetBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id_, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id_, name.c_str()), value);
}

void Shader::SetVec2(const std::string & name, glm::vec2 value) const
{
	glUniform2f(glGetUniformLocation(m_id_, name.c_str()), value.x, value.y);
}

void Shader::SetVec3(const std::string & name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(m_id_, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string & name, glm::vec4 value) const
{
	glUniform4f(glGetUniformLocation(m_id_, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat2(const std::string & name, glm::mat2 value) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3(const std::string & name, glm::mat3 value) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string & name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
