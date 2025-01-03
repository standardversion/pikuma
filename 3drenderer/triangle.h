#pragma once
#include "vector"
#include "vector2d.h"

namespace tri
{
	struct face_t
	{
		int a;
		int b;
		int c;
	};

	struct triangle_t
	{
		//direct (parenthesis) initialization is disallowed for member default initializers.
		//std::vector<vector::Vector2d> points(3) won't work
		std::vector<vector::Vector2d> points{ std::vector<vector::Vector2d>(3) };
	};
}
