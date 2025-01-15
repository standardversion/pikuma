#pragma once
#include "vector"
#include "vector2d.h"

namespace geo
{
	struct face_t
	{
		int a;
		int b;
		int c;
	};

	class Triangle
	{
	public:
		Triangle();
		Triangle(std::vector<vector::Vector2d>& points);

		std::vector<vector::Vector2d> m_points;
	};
}
