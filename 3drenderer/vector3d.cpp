#include "vector3d.h"
#include <math.h>

namespace vector
{
	Vector3d::Vector3d(double x, double y, double z)
		: m_x{x}, m_y{y}, m_z{z}
	{ }

	double Vector3d::get_x() const
	{
		return m_x;
	}

	double Vector3d::get_y() const
	{
		return m_y;
	}

	double Vector3d::get_z() const
	{
		return m_z;
	}

	void Vector3d::set_x(double x)
	{
		m_x = x;
	}

	void Vector3d::set_y(double y)
	{
		m_y = y;
	}

	void Vector3d::set_z(double z)
	{
		m_z = z;
	}

	void Vector3d::rotate_x(double angle)
	{
		double y{ m_y * cos(angle) - m_z * sin(angle) };
		double z{ m_y * sin(angle) + m_z * cos(angle) };
		m_y = y;
		m_z = z;
	}

	void Vector3d::rotate_y(double angle)
	{
		double x{ m_x * cos(angle) - m_z * sin(angle) };
		double z{ m_x * sin(angle) + m_z * cos(angle) };
		m_x = x;
		m_z = z;
	}

	void Vector3d::rotate_z(double angle)
	{
		double x{ m_x * cos(angle) - m_y * sin(angle) };
		double y{ m_x * sin(angle) + m_y * cos(angle) };
		m_x = x;
		m_y = y;
	}

	Vector2d Vector3d::project(double fov_factor)
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