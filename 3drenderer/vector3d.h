#pragma once
#include "vector2d.h"


namespace vector
{
	class Vector3d
	{
	public:
		Vector3d(const double x = 0.0, const double y = 0.0, const double z = 0.0);
		void rotate_x(const double angle);
		void rotate_y(const double angle);
		void rotate_z(const double angle);
		Vector2d project(const double fov_factor = 1.0) const;

		double m_x{0.0};
		double m_y{0.0};
		double m_z{0.0};
	};
}
