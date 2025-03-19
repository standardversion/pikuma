#pragma once
#include <math.h>

namespace vector
{
	template <typename T>
	class Vector2d
	{
	public:
		Vector2d(const T x, const T y);
		static double get_scalar_factor(const Vector2d<T>& a, const Vector2d<T>& b, const Vector2d<T>& c);
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

	/*
	This function computes a scalar factor that relates the distance between two points (a and b) to the distance between two other points (a and c).
	It uses the Euclidean distance formula to calculate the distances and returns their ratio as a scalar factor.
	*/
	template <typename T>
	double Vector2d<T>::get_scalar_factor(const Vector2d<T>& a, const Vector2d<T>& b, const Vector2d<T>& c)
	{
		double ac{ sqrt((c.m_x - a.m_x) * (c.m_x - a.m_x) + (c.m_y - a.m_y) * (c.m_y - a.m_y)) };
		double ab{ sqrt((b.m_x - a.m_x) * (b.m_x - a.m_x) + (b.m_y - a.m_y) * (b.m_y - a.m_y)) };
		return ab / ac;

	}

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