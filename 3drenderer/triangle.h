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
		Triangle<T>() {};
		Triangle<T>(std::vector<vector::Vector2d<T>>& points);
		Triangle<T>(vector::Vector2d<T>& a, vector::Vector2d<T>& b, vector::Vector2d<T>& c);
		void sort_vertices_by_y();
		vector::Vector2d<T> get_midpoint() const;
		double get_light_intensity_at_mp() const;
		double get_inverse_slope(std::size_t lower_pt_index, std::size_t higher_pt_index) const;

		std::vector<vector::Vector2d<T>> m_points;
		double m_avg_depth{0.0};
		vector::Vector2d<double> m_center{ 0.0, 0.0 };
		vector::Vector2d<double> m_face_normal{ 0.0, 0.0 };
		std::vector<double> m_per_vtx_lt_intensity{};
	};

	template <typename T>
	Triangle<T>::Triangle(std::vector<vector::Vector2d<T>>& points)
		: m_points{ points }
	{

	};

	template <typename T>
	Triangle<T>::Triangle(vector::Vector2d<T>& a, vector::Vector2d<T>& b, vector::Vector2d<T>& c)
		: m_points{ std::vector <vector::Vector2d<T>>{ a, b, c } }
	{

	};

	template <typename T>
	void Triangle<T>::sort_vertices_by_y()
	{
		// sort the per vertex light intensities as well
		vector::Vector2d<T> point0{ m_points[0] };
		vector::Vector2d<T> point1{ m_points[1] };
		vector::Vector2d<T> point2{ m_points[2] };
		double lt_intensity_p0{ m_per_vtx_lt_intensity[0] };
		double lt_intensity_p1{ m_per_vtx_lt_intensity[1] };
		double lt_intensity_p2{ m_per_vtx_lt_intensity[2] };

		if (point1.m_y < point0.m_y)
		{
			m_points[0] = point1;
			m_points[1] = point0;
			m_per_vtx_lt_intensity[0] = lt_intensity_p1;
			m_per_vtx_lt_intensity[1] = lt_intensity_p0;
			point0 = m_points[0];
			point1 = m_points[1];
			point2 = m_points[2];
			lt_intensity_p0 = m_per_vtx_lt_intensity[0];
			lt_intensity_p1 = m_per_vtx_lt_intensity[1];
			lt_intensity_p2 = m_per_vtx_lt_intensity[2];
		}
		if (point2.m_y < point0.m_y)
		{
			m_points[0] = point2;
			m_points[2] = point0;
			m_per_vtx_lt_intensity[0] = lt_intensity_p2;
			m_per_vtx_lt_intensity[2] = lt_intensity_p0;
			point0 = m_points[0];
			point1 = m_points[1];
			point2 = m_points[2];
			lt_intensity_p0 = m_per_vtx_lt_intensity[0];
			lt_intensity_p1 = m_per_vtx_lt_intensity[1];
			lt_intensity_p2 = m_per_vtx_lt_intensity[2];
		}
		if (point2.m_y < point1.m_y)
		{
			m_points[1] = point2;
			m_points[2] = point1;
			m_per_vtx_lt_intensity[1] = lt_intensity_p2;
			m_per_vtx_lt_intensity[2] = lt_intensity_p1;
		}
	}

	template <typename T>
	vector::Vector2d<T> Triangle<T>::get_midpoint() const
	{
		T x{ (((m_points[2].m_x - m_points[0].m_x) * (m_points[1].m_y - m_points[0].m_y)) / (m_points[2].m_y - m_points[0].m_y)) + m_points[0].m_x };
		return vector::Vector2d<T>{ x, m_points[1].m_y };
	}

	///////////////////////////////////////////////////////////////////////////////
	//
	//          (x0,y0)
	//            / \
	//           /   \
	//          /     \
	//         /       \
	//        /         \
	//   (x1,y1)------(Mx,My)
	//       \_           \
	//          \_         \
	//             \_       \
	//                \_     \
	//                   \    \
	//                     \_  \
	//                        \_\
	//                           \
	//                         (x2,y2)
	//
	///////////////////////////////////////////////////////////////////////////////
	template <typename T>
	double Triangle<T>::get_light_intensity_at_mp() const
	{
		T delta_y{ m_points[2].m_y - m_points[0].m_y };
		double delta_lt_intensity_y{ m_per_vtx_lt_intensity[2] - m_per_vtx_lt_intensity[0] };
		double delta_lt_intensity_per_y{ delta_lt_intensity_y / static_cast<double>(delta_y) };
		return static_cast<double>(delta_lt_intensity_per_y * (delta_y / 2.0));
	}

	template <typename T>
	double Triangle<T>::get_inverse_slope(std::size_t lower_pt_index, std::size_t higher_pt_index) const
	{
		return (m_points[lower_pt_index].m_x - m_points[higher_pt_index].m_x) / static_cast<double>(m_points[lower_pt_index].m_y - m_points[higher_pt_index].m_y);
	}
}
