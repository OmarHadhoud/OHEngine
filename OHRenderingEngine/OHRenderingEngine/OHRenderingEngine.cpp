
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	glfwInit();
	glfwInitHint(GL_MINOR_VERSION, 3);
	glfwInitHint(GL_MAJOR_VERSION, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(720, 480, "Test", nullptr, nullptr);
	glfwTerminate();
	return 0;
}

