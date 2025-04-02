#pragma once
#include "vector2d.h"

// if we include vector4d.h then we'll end up with circular dependency
namespace vector
{
	class Vector4d;
}


namespace vector
{
	class Vector3d
	{
	public:
		Vector3d(const double x = 0.0, const double y = 0.0, const double z = 0.0);
		Vector3d(const Vector2d<double>& vec2d);
		Vector3d(const Vector4d& vec4d);
		double get_magnitude() const;
		Vector3d& operator+=(const Vector3d& v);
		Vector3d operator+(const Vector3d& v) const;
		Vector3d& operator-=(const Vector3d& v);
		Vector3d operator-(const Vector3d& v) const;
		Vector3d operator*(double factor) const;
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
