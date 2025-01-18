#pragma once
#include <math.h>

namespace vector
{
	template <typename T>
	class Vector2d
	{
	public:
		Vector2d(const T x, const T y);
		double get_magnitude() const;
		Vector2d<T>& operator+=(const Vector2d<T>& v);
		Vector2d<T> operator+(const Vector2d<T>& v) const;
		Vector2d<T>& operator-=(const Vector2d<T>& v);
		Vector2d<T> operator-(const Vector2d<T>& v) const;
		Vector2d<T>& operator*=(double factor);;
		Vector2d<T>& operator/=(double factor);;
		void normalize();;
		double dot_product(const Vector2d<T>& v) const;
		void rotate(const double angle);

		T m_x{ };
		T m_y{ };
	};

	template <typename T>
	Vector2d<T>::Vector2d(const T x, const T y)
		: m_x{ x }, m_y{ y }
	{
	};

	template <typename T>
	double Vector2d<T>::get_magnitude() const
	{
		return sqrt(m_x * m_x + m_y * m_y);
	};

	template <typename T>
	Vector2d<T>& Vector2d<T>::operator+=(const Vector2d<T>& v)
	{
		this->m_x += v.m_x;
		this->m_y += v.m_y;
		return *this;
	};

	template <typename T>
	Vector2d<T> Vector2d<T>::operator+(const Vector2d<T>& v) const
	{
		return Vector2d<T>{ m_x + v.m_x, m_y + v.m_y };
	};

	template <typename T>
	Vector2d<T>& Vector2d<T>::operator-=(const Vector2d<T>& v)
	{
		this->m_x -= v.m_x;
		this->m_y -= v.m_y;
		return *this;
	};

	template <typename T>
	Vector2d<T> Vector2d<T>::operator-(const Vector2d<T>& v) const
	{
		return Vector2d<T>{ m_x - v.m_x, m_y - v.m_y };
	};

	template <typename T>
	Vector2d<T>& Vector2d<T>::operator*=(double factor)
	{
		this->m_x *= factor;
		this->m_y *= factor;
		return *this;
	};

	template <typename T>
	Vector2d<T>& Vector2d<T>::operator/=(double factor)
	{
		this->m_x /= factor;
		this->m_y /= factor;
		return *this;
	};

	template <typename T>
	void Vector2d<T>::normalize()
	{
		double mag{ get_magnitude() };
		m_x /= mag;
		m_y /= mag;
	};

	template <typename T>
	double Vector2d<T>::dot_product(const Vector2d<T>& v) const
	{
		return (m_x * v.m_x) + (m_y * v.m_y);
	}

	template <typename T>
	void Vector2d<T>::rotate(const double angle)
	{
		double x{ m_x * cos(angle) - m_y * sin(angle) };
		double y{ m_x * sin(angle) + m_y * cos(angle) };
		m_x = x;
		m_y = y;
	}
}