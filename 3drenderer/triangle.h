#pragma once
#include "vector"
#include "vector2d.h"

namespace geo
{
	struct face_t
	{
		int a;
		int b;
		int c;
	};

	template <typename T>
	class Triangle
	{
	public:
		Triangle<T>()
		{

		};
		Triangle<T>(std::vector<vector::Vector2d<T>>& points)
			: m_points{ points }
		{

		};

		Triangle<T>(vector::Vector2d<T>& a, vector::Vector2d<T>& b , vector::Vector2d<T>& c)
			: m_points{ std::vector <vector::Vector2d<T>>{ a, b, c } }
		{

		};

		void sort()
		{
			vector::Vector2d<T> point0{ m_points[0] };
			vector::Vector2d<T> point1{ m_points[1] };
			vector::Vector2d<T> point2{ m_points[2] };
			if (point1.m_y < point0.m_y)
			{
				m_points[0] = point1;
				m_points[1] = point0;
				point0 = m_points[0];
				point1 = m_points[1];
				point2 = m_points[2];
			}
			if (point2.m_y < point0.m_y)
			{
				m_points[0] = point2;
				m_points[2] = point0;
				point0 = m_points[0];
				point1 = m_points[1];
				point2 = m_points[2];
			}
			if (point2.m_y < point1.m_y)
			{
				m_points[1] = point2;
				m_points[2] = point1;
			}
		}

		vector::Vector2d<T> get_midpoint() const
		{
			T x{ (((m_points[2].m_x - m_points[0].m_x) * (m_points[1].m_y - m_points[0].m_y)) / (m_points[2].m_y - m_points[0].m_y)) + m_points[0].m_x };
			return vector::Vector2d<T>{ x, m_points[1].m_y };
		}
		float get_inverse_slope(std::size_t lower_pt_index, std::size_t higher_pt_index) const
		{
			return (m_points[lower_pt_index].m_x - m_points[higher_pt_index].m_x) / static_cast<float>(m_points[lower_pt_index].m_y - m_points[higher_pt_index].m_y);
		}


		std::vector<vector::Vector2d<T>> m_points;
	};
}
