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

enum ComparisonFunc 
{
	kLess = GL_LESS,
	kEqual = GL_EQUAL,
	kLEqual = GL_LEQUAL,
	kGreater = GL_GREATER,
	kGEqual = GL_GEQUAL,
	kAlways = GL_ALWAYS,
	kNever = GL_NEVER,
	kNotEqual = GL_NOTEQUAL
};

enum TestOption
{
	kKeep = GL_KEEP,
	kZero = GL_ZERO,
	kReplace = GL_REPLACE,
	kIncr = GL_INCR,
	KIncrWrap = GL_INCR_WRAP,
	kDecr = GL_DECR,
	kDecrWrap = GL_DECR_WRAP,
	kInvert = GL_INVERT
};

#endif // !ENUMS_H

