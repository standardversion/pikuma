#pragma once

namespace vector
{
	class Vector2d
	{
	public:
		Vector2d(const double x = 0.0, const double y = 0.0);

		double m_x{ 0.0 };
		double m_y{ 0.0 };
	};
}