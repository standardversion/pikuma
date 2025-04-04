#include <math.h>
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

	v2d v2d::rotate(const v2d& center, double angle) const
	{
		v2d rot;
		v2d v{ x - center.x, y - center.y };
		rot.x = v.x * cos(angle) - v.y * sin(angle);
		rot.y = v.x * sin(angle) + v.y * cos(angle);
		rot.x += center.x;
		rot.y += center.y;
		return rot;
	}
}