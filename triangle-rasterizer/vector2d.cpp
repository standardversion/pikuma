#include "vector2d.h"

namespace vec
{
	v2d v2d::operator-(const v2d& v) const
	{
		return v2d{ x - v.x, y - v.y };
	}

	double v2d::cross_product(const v2d& v) const
	{
		return  x * v.y - y * v.x;
	}
}