#pragma once
#include <glm/vec2.hpp>
//Comparetor for vector2 in glm
namespace glm {
	template <typename T, precision P>
	bool operator<(const tvec2<T, P>& a, const tvec2<T, P>& b)
	{
		return (a.x < b.x || (a.x == b.x && a.y < b.y));
	}
};