#pragma once

namespace vec
{
	struct v2d
	{
		double x{ 0.0 };
		double y{ 0.0 };

		v2d operator-(const v2d& v) const;
		double cross_product(const v2d& v) const;
	};
}
