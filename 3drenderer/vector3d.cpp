#include "vector3d.h"
#include <math.h>

namespace vector
{
	Vector3d::Vector3d(const double x, const double y, const double z)
		: m_x{x}, m_y{y}, m_z{z}
	{ }

	void Vector3d::rotate_x(const double angle)
	{
		double y{ m_y * cos(angle) - m_z * sin(angle) };
		double z{ m_y * sin(angle) + m_z * cos(angle) };
		m_y = y;
		m_z = z;
	}

	void Vector3d::rotate_y(const double angle)
	{
		double x{ m_x * cos(angle) - m_z * sin(angle) };
		double z{ m_x * sin(angle) + m_z * cos(angle) };
		m_x = x;
		m_z = z;
	}

	void Vector3d::rotate_z(const double angle)
	{
		double x{ m_x * cos(angle) - m_y * sin(angle) };
		double y{ m_x * sin(angle) + m_y * cos(angle) };
		m_x = x;
		m_y = y;
	}

	Vector2d Vector3d::project(const double fov_factor) const
	{
		// implementing perspective divide
		// P'x = Px     P'y = Py
		//     -----         -----
		//       Pz           Pz
		return Vector2d{ (m_x * fov_factor) / m_z, (m_y * fov_factor) / m_z };
		// ortho projection:
		//return Vector2d{ (m_x * fov_factor), (m_y * fov_factor) };
	}
}