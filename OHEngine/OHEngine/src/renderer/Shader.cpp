#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Debugger.h"

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
	GlCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
	GlCall(glCompileShader(vertex));
	// print compile errors if any
	GlCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GlCall(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GlCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
	GlCall(glCompileShader(fragment));
	// print compile errors if any
	GlCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GlCall(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	// shader Program
	m_ID = glCreateProgram();
	GlCall(glAttachShader(m_ID, vertex));
	GlCall(glAttachShader(m_ID, fragment));
	GlCall(glLinkProgram(m_ID));
	// print linking errors if any
	GlCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &success));
	if (!success)
	{
		GlCall(glGetProgramInfoLog(m_ID, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n\t" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	GlCall(glDeleteShader(vertex));
	GlCall(glDeleteShader(fragment));
	Use();
}

Shader::Shader(const char * vertexPath, const char * fragmentPath, const char* geomtryPath)
{
	//1-Retrieve the vertex, geomtry and fragment source code from file path
	std::string vertexCode;
	std::string fragmentCode;
	std::string geomtryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	//Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		gShaderFile.open(geomtryPath);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		//read file buffer into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		//close file handlers
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();
		//convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geomtryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geomtryCode.c_str();

	//2-Compile shaders
	unsigned int vertex, fragment, geomtry;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	GlCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
	GlCall(glCompileShader(vertex));
	// print compile errors if any
	GlCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GlCall(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GlCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
	GlCall(glCompileShader(fragment));
	// print compile errors if any
	GlCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GlCall(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	// geomtry Shader
	geomtry = glCreateShader(GL_GEOMETRY_SHADER);
	GlCall(glShaderSource(geomtry, 1, &gShaderCode, NULL));
	GlCall(glCompileShader(geomtry));
	// print compile errors if any
	GlCall(glGetShaderiv(geomtry, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GlCall(glGetShaderInfoLog(geomtry, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::GEOMTRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	// shader Program
	m_ID = glCreateProgram();
	GlCall(glAttachShader(m_ID, vertex));
	GlCall(glAttachShader(m_ID, fragment));
	GlCall(glAttachShader(m_ID, geomtry));
	GlCall(glLinkProgram(m_ID));
	// print linking errors if any
	GlCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &success));
	if (!success)
	{
		GlCall(glGetProgramInfoLog(m_ID, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n\t" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	GlCall(glDeleteShader(vertex));
	GlCall(glDeleteShader(fragment));
	GlCall(glDeleteShader(geomtry));
	Use();
}

Shader::~Shader()
{
	GlCall(glDeleteProgram(m_ID));
}

void Shader::Use() const
{
	//Activate this shader
	GlCall(glUseProgram(this->m_ID));
}

unsigned int Shader::GetID() const
{
	return m_ID;
}


void Shader::SetBool(const std::string & name, bool value) const
{
	GlCall(glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value));
}

void Shader::SetInt(const std::string & name, int value) const
{
	GlCall(glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value));
}

void Shader::SetFloat(const std::string & name, float value) const
{
	GlCall(glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value));
}

void Shader::SetVec2(const std::string & name, glm::vec2 value) const
{
	GlCall(glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y));
}

void Shader::SetVec3(const std::string & name, glm::vec3 value) const
{
	GlCall(glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z));
}

void Shader::SetVec4(const std::string & name, glm::vec4 value) const
{
	GlCall(glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w));
}

void Shader::SetMat2(const std::string & name, glm::mat2 value) const
{
	GlCall(glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::SetMat3(const std::string & name, glm::mat3 value) const
{
	GlCall(glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::SetMat4(const std::string & name, glm::mat4 value) const
{
	GlCall(glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)));
}
