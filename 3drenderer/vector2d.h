#pragma once

namespace vector
{
	class Vector2d
	{
	public:
		Vector2d(const double x = 0.0, const double y = 0.0);
		double get_magnitude() const;
		Vector2d& operator+=(const Vector2d& v);
		Vector2d operator+(const Vector2d& v) const;
		Vector2d& operator-=(const Vector2d& v);
		Vector2d operator-(const Vector2d& v) const;
		Vector2d& operator*=(double factor);
		Vector2d& operator/=(double factor);
		void normalize();
		double dot_product(const Vector2d& v) const;
		void rotate(const double angle);

		double m_x{ 0.0 };
		double m_y{ 0.0 };
	};
}