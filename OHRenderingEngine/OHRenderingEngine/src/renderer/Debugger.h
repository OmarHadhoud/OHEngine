#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <intrin.h>

#include <iostream>

#include <glad/glad.h>

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef _DEBUG

#define GlCall(x)\
GlClearError();\
x;\
ASSERT(GlCheckError(#x, __FILE__, __LINE__));

#else

#define GlCall(x) x;

#endif

//Clears all errors in openGL that are waiting
static void GlClearError()
{
	while (glGetError() != GL_NO_ERROR) {}
}

//Runs the function and outputs error if there is any, if in debug mode
static bool GlCheckError(const char* fun_name, const char* path, int line_num)
{
	GLenum err = glGetError();
	if (err == GL_NO_ERROR) return true;
	while (err != GL_NO_ERROR)
	{
		std::cout << "OPENGL ERROR: " << err << " AT " << fun_name << " IN " << path << " IN LINE " << line_num << std::endl;
		err = glGetError();
	}
	return false;
}

#endif	//DEBUGGER_H
