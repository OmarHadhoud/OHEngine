#ifndef ENUMS_H
#define ENUMS_H

#include <glad/glad.h>

enum BUFFER_USAGE {
	kStaticDraw = GL_STATIC_DRAW,
	kStreamDraw = GL_STREAM_DRAW,
	kDynamicDraw = GL_DYNAMIC_DRAW
};

enum MovementDirection
{
	kForward = 0,
	kBackward = 1,
	kRight = 2,
	kLeft = 3
};


#endif // !ENUMS_H

