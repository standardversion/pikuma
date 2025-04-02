#pragma once

// if we include vector3d.h then we'll end up with circular dependency
namespace vector
{
	class Vector3d;
}


namespace vector
{
	class Vector4d
	{
	public:
		Vector4d(const double x = 0.0, const double y = 0.0, const double z = 0.0, const double w = 1.0);
		Vector4d(const Vector3d& vec3d);

		double dot_product(const Vector4d& v) const;

		double m_x{ 0.0 };
		double m_y{ 0.0 };
		double m_z{ 0.0 };
		double m_w{ 1.0 };
	};
}