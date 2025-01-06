#include "vector3d.h"
#include <math.h>

namespace vector
{
	Vector3d::Vector3d(const double x, const double y, const double z)
		: m_x{x}, m_y{y}, m_z{z}
	{ }

	double Vector3d::get_magnitude() const
	{
		return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
	}

	Vector3d& Vector3d::operator+=(const Vector3d& v)
	{
		this->m_x += v.m_x;
		this->m_y += v.m_y;
		this->m_z += v.m_z;
		return *this;
	}

	Vector3d Vector3d::operator+(const Vector3d& v) const
	{
		return Vector3d{ m_x + v.m_x, m_y + v.m_y, m_z + v.m_z};
	}

	Vector3d& Vector3d::operator-=(const Vector3d& v)
	{
		this->m_x -= v.m_x;
		this->m_y -= v.m_y;
		this->m_z -= v.m_z;
		return *this;
	}

	Vector3d Vector3d::operator-(const Vector3d& v) const
	{
		return Vector3d{ m_x - v.m_x, m_y - v.m_y, m_z - v.m_z };
	}

	Vector3d& Vector3d::operator*=(double factor)
	{
		this->m_x *= factor;
		this->m_y *= factor;
		this->m_z *= factor;
		return *this;
	}

	Vector3d& Vector3d::operator/=(double factor)
	{
		this->m_x /= factor;
		this->m_y /= factor;
		this->m_z /= factor;
		return *this;
	}

	void Vector3d::normalize()
	{
		double mag{ get_magnitude() };
		m_x /= mag;
		m_y /= mag;
		m_z /= mag;
	}

	Vector3d Vector3d::cross_product(const Vector3d& v) const
	{
		return Vector3d{
			m_y * v.m_z - m_z * v.m_y,
			m_z * v.m_x - m_x * v.m_z,
			m_x * v.m_y - m_y * v.m_x
		};
	}

	double Vector3d::dot_product(const Vector3d& v) const
	{
		return (m_x * v.m_x) + (m_y * v.m_y) + (m_z * v.m_z);
	}

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