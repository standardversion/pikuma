#pragma once
#include "vector2d.h"


namespace vector
{
	class Vector3d
	{
	public:
		Vector3d(double x, double y, double z);
		double get_x() const;
		double get_y() const;
		double get_z() const;
		void set_x(double x);
		void set_y(double y);
		void set_z(double z);
		void rotate_x(double angle);
		void rotate_y(double angle);
		void rotate_z(double angle);
		Vector2d project(double fov_factor = 1.0);
	private:
		double m_x{};
		double m_y{};
		double m_z{};
	};
}
