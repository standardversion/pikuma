#include <math.h>
#include "vector2d.h"

namespace vector
{
	Vector2d::Vector2d(double x, double y)
		: m_x{x}, m_y{y}
	{ }

	double Vector2d::get_magnitude() const
	{
		return sqrt(m_x * m_x + m_y * m_y);
	}

	Vector2d& Vector2d::operator+=(const Vector2d& v)
	{
		this->m_x += v.m_x;
		this->m_y += v.m_y;
		return *this;
	}

	Vector2d Vector2d::operator+(const Vector2d& v) const
	{
		return Vector2d{ m_x + v.m_x, m_y + v.m_y };
	}

	Vector2d& Vector2d::operator-=(const Vector2d& v)
	{
		this->m_x -= v.m_x;
		this->m_y -= v.m_y;
		return *this;
	}

	Vector2d Vector2d::operator-(const Vector2d& v) const
	{
		return Vector2d{ m_x - v.m_x, m_y - v.m_y };
	}

	Vector2d& Vector2d::operator*=(double factor)
	{
		this->m_x *= factor;
		this->m_y *= factor;
		return *this;
	}

	Vector2d& Vector2d::operator/=(double factor)
	{
		this->m_x /= factor;
		this->m_y /= factor;
		return *this;
	}

	void Vector2d::normalize()
	{
		double mag{ get_magnitude() };
		m_x /= mag;
		m_y /= mag;
	}

	double Vector2d::dot_product(const Vector2d& v) const
	{
		return (m_x * v.m_x) + (m_y * v.m_y);
	}

	void Vector2d::rotate(const double angle)
	{
		double x{ m_x * cos(angle) - m_y * sin(angle) };
		double y{ m_x * sin(angle) + m_y * cos(angle) };
		m_x = x;
		m_y = y;
	}
}