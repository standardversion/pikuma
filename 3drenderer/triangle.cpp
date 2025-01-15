#include "triangle.h"

namespace geo
{
	Triangle::Triangle()
	{

	}

	Triangle::Triangle(std::vector<vector::Vector2d>& points)
		: m_points{ points }
	{

	}
}