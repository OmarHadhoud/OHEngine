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

enum WrapDir
{
	kS = GL_TEXTURE_WRAP_S,
	kR = GL_TEXTURE_WRAP_R,
	kT = GL_TEXTURE_WRAP_T
};

enum WrapType
{
	kRepeat = GL_REPEAT,
	kMirroredRepeat = GL_MIRRORED_REPEAT,
	kClampToEdge = GL_CLAMP_TO_EDGE,
	kClampToBorder = GL_CLAMP_TO_BORDER
};

enum TextureFilter
{
	kNearest = GL_NEAREST,
	kLinear = GL_LINEAR,
	kNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	kNearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	kLinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	kLinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR ,
};

enum BufferType
{
	kColor,
	kDepth,
	kStencil,
	kDepthStencil,
	kColorF
};

enum FrameBufferTarget
{
	kRead = GL_READ_FRAMEBUFFER,
	kDraw = GL_DRAW_FRAMEBUFFER,
	kReadDraw = GL_FRAMEBUFFER
};

enum FrameBufferAttachement
{
	kColorAttach0 = GL_COLOR_ATTACHMENT0,
	kColorAttach1 = GL_COLOR_ATTACHMENT1,
	kColorAttach2 = GL_COLOR_ATTACHMENT2,
	kColorAttach3 = GL_COLOR_ATTACHMENT3,
	kColorAttach4 = GL_COLOR_ATTACHMENT4,
	kColorAttach5 = GL_COLOR_ATTACHMENT5,
	kColorAttach6 = GL_COLOR_ATTACHMENT6,
	kColorAttach7 = GL_COLOR_ATTACHMENT7,
	kColorAttach8 = GL_COLOR_ATTACHMENT8,
	kColorAttach9 = GL_COLOR_ATTACHMENT9,
	kColorAttach10 = GL_COLOR_ATTACHMENT10,
	kColorAttach11 = GL_COLOR_ATTACHMENT11,
	kColorAttach12 = GL_COLOR_ATTACHMENT12,
	kColorAttach13 = GL_COLOR_ATTACHMENT13,
	kColorAttach14 = GL_COLOR_ATTACHMENT14,
	kColorAttach15 = GL_COLOR_ATTACHMENT15,
	kColorAttach16 = GL_COLOR_ATTACHMENT16,
	kColorAttach17 = GL_COLOR_ATTACHMENT17,
	kColorAttach18 = GL_COLOR_ATTACHMENT18,
	kColorAttach19 = GL_COLOR_ATTACHMENT19,
	kColorAttach20 = GL_COLOR_ATTACHMENT20,
	kColorAttach21 = GL_COLOR_ATTACHMENT21,
	kColorAttach22 = GL_COLOR_ATTACHMENT22,
	kColorAttach23 = GL_COLOR_ATTACHMENT23,
	kColorAttach24 = GL_COLOR_ATTACHMENT24,
	kColorAttach25 = GL_COLOR_ATTACHMENT25,
	kColorAttach26 = GL_COLOR_ATTACHMENT26,
	kColorAttach27 = GL_COLOR_ATTACHMENT27,
	kColorAttach28 = GL_COLOR_ATTACHMENT28,
	kColorAttach29 = GL_COLOR_ATTACHMENT29,
	kColorAttach30 = GL_COLOR_ATTACHMENT30,
	kColorAttach31 = GL_COLOR_ATTACHMENT31,
	kDepthAttach = GL_DEPTH_ATTACHMENT,
	kStencilAttach = GL_STENCIL_ATTACHMENT,
	kDepthStencilAttach = GL_DEPTH_STENCIL_ATTACHMENT,
	kNone = GL_NONE
};

enum TextureType
{
	k2D = GL_TEXTURE_2D,
	k2DMS = GL_TEXTURE_2D_MULTISAMPLE,
	kCubeMap = GL_TEXTURE_CUBE_MAP
};


enum TextureFormat
{
	kRGB = GL_RGB,
	kRGBA = GL_RGBA,
	kSRGB = GL_SRGB,
	kSRGBA = GL_SRGB_ALPHA
};

enum kBitMask
{
	kColorBufferBit = GL_COLOR_BUFFER_BIT,
	kDepthBufferBit = GL_DEPTH_BUFFER_BIT,
	kStencilBufferBit = GL_STENCIL_BUFFER_BIT
};

enum kBlendFactor
{
	//kZero is already defined, thus we don't define it here.
	KOne = GL_ONE,
	kSrcColor = GL_SRC_COLOR,
	kOneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
	kDestColor = GL_DST_COLOR,
	kOneMinusDestColor = GL_ONE_MINUS_DST_COLOR,
	kSrcAlpha = GL_SRC_ALPHA,
	kOneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
	kDestAlpha = GL_DST_ALPHA,
	kOneMinusDestAlpha = GL_ONE_MINUS_DST_ALPHA,
	kConstColor = GL_CONSTANT_COLOR,
	kOneMinusConstColor = GL_ONE_MINUS_CONSTANT_COLOR,
	kConstAlpha = GL_CONSTANT_ALPHA,
	kOneMinusConstAlpha = GL_ONE_MINUS_CONSTANT_ALPHA
};

enum kBlendFunc
{
	kAdd = GL_FUNC_ADD,
	kSubtract = GL_FUNC_SUBTRACT,
	kRevSubtract = GL_FUNC_REVERSE_SUBTRACT,
	kMin = GL_MIN,
	kMax = GL_MAX
};

enum kFace
{
	kBack = GL_BACK,
	kFront = GL_FRONT,
	kFrontAndBack = GL_FRONT_AND_BACK
};

enum kWinding
{
	kCW = GL_CW,
	kCCW = GL_CCW
};
#endif // !ENUMS_H

