#pragma once

namespace vector
{
	class Vector2d
	{
	public:
		Vector2d(double x = 1.0, double y = 1.0);
		double get_x() const;
		double get_y() const;
		void set_x(double x);
		void set_y(double y);
	private:
		double m_x{ 1.0 };
		double m_y{ 1.0 };
	};
}