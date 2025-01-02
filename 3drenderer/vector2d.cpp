#include "vector2d.h"

namespace vector
{
	Vector2d::Vector2d(double x, double y)
		: m_x{x}, m_y{y}
	{ }

	double Vector2d::get_x() const
	{
		return m_x;
	}

	double Vector2d::get_y() const
	{
		return m_y;
	}

	void Vector2d::set_x(double x)
	{
		m_x = x;
	}

	void Vector2d::set_y(double y)
	{
		m_y = y;
	}
}