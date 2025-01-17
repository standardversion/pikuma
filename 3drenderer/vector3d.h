#pragma once
#include "vector2d.h"


namespace vector
{
	class Vector3d
	{
	public:
		Vector3d(const double x = 0.0, const double y = 0.0, const double z = 0.0);
		double get_magnitude() const;
		Vector3d& operator+=(const Vector3d& v);
		Vector3d operator+(const Vector3d& v) const;
		Vector3d& operator-=(const Vector3d& v);
		Vector3d operator-(const Vector3d& v) const;
		Vector3d& operator*=(double factor);
		Vector3d& operator/=(double factor);
		void normalize();
		Vector3d cross_product(const Vector3d& v) const;
		double dot_product(const Vector3d& v) const;
		void rotate_x(const double angle);
		void rotate_y(const double angle);
		void rotate_z(const double angle);
		Vector2d<double> project(const double fov_factor = 1.0) const;

		double m_x{0.0};
		double m_y{0.0};
		double m_z{0.0};
	};
}
