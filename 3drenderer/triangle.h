#pragma once
#include "vector"
#include "vector2d.h"
#include "vector3d.h"

namespace geo
{
	struct face_t
	{
		int a;
		int b;
		int c;
		int a_uv;
		int b_uv;
		int c_uv;
		int a_normal;
		int b_normal;
		int c_normal;
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
		double get_inverse_slope(std::size_t lower_pt_index, std::size_t higher_pt_index) const;
		vector::Vector3d get_barycentric_weights(const vector::Vector2d<T>& p) const;

		std::vector<vector::Vector2d<T>> m_points;
		std::vector<vector::Vector2d<double>> m_uvs;
		double m_avg_depth{0.0};
		double m_light_intensity{ 1.0 };
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
		vector::Vector2d<T> point0{ m_points[0] };
		vector::Vector2d<T> point1{ m_points[1] };
		vector::Vector2d<T> point2{ m_points[2] };
		vector::Vector2d<double> uv0{ m_uvs[0] };
		vector::Vector2d<double> uv1{ m_uvs[1] };
		vector::Vector2d<double> uv2{ m_uvs[2] };
		double intensity_p0{ m_per_vtx_lt_intensity[0] };
		double intensity_p1{ m_per_vtx_lt_intensity[1] };
		double intensity_p2{ m_per_vtx_lt_intensity[2] };

		if (point1.m_y < point0.m_y)
		{
			m_points[0] = point1;
			m_points[1] = point0;
			m_per_vtx_lt_intensity[0] = intensity_p1;
			m_per_vtx_lt_intensity[1] = intensity_p0;
			m_uvs[0] = uv1;
			m_uvs[1] = uv0;
			point0 = m_points[0];
			point1 = m_points[1];
			point2 = m_points[2];
			uv0 = m_uvs[0];
			uv1 = m_uvs[1];
			uv2 = m_uvs[2];
			intensity_p0 = m_per_vtx_lt_intensity[0];
			intensity_p1 = m_per_vtx_lt_intensity[1];
			intensity_p2 = m_per_vtx_lt_intensity[2];
		}
		if (point2.m_y < point0.m_y)
		{
			m_points[0] = point2;
			m_points[2] = point0;
			m_uvs[0] = uv2;
			m_uvs[2] = uv0;
			m_per_vtx_lt_intensity[0] = intensity_p2;
			m_per_vtx_lt_intensity[2] = intensity_p0;
			point0 = m_points[0];
			point1 = m_points[1];
			point2 = m_points[2];
			uv0 = m_uvs[0];
			uv1 = m_uvs[1];
			uv2 = m_uvs[2];
			intensity_p0 = m_per_vtx_lt_intensity[0];
			intensity_p1 = m_per_vtx_lt_intensity[1];
			intensity_p2 = m_per_vtx_lt_intensity[2];
		}
		if (point2.m_y < point1.m_y)
		{
			m_points[1] = point2;
			m_points[2] = point1;
			m_uvs[1] = uv2;
			m_uvs[2] = uv1;
			m_per_vtx_lt_intensity[1] = intensity_p2;
			m_per_vtx_lt_intensity[2] = intensity_p1;
		}
	}

	template <typename T>
	vector::Vector2d<T> Triangle<T>::get_midpoint() const
	{
		T x{ (((m_points[2].m_x - m_points[0].m_x) * (m_points[1].m_y - m_points[0].m_y)) / (m_points[2].m_y - m_points[0].m_y)) + m_points[0].m_x };
		return vector::Vector2d<T>{ x, m_points[1].m_y };
	}

	template <typename T>
	double Triangle<T>::get_inverse_slope(std::size_t lower_pt_index, std::size_t higher_pt_index) const
	{
		return (m_points[lower_pt_index].m_x - m_points[higher_pt_index].m_x) / static_cast<double>(m_points[lower_pt_index].m_y - m_points[higher_pt_index].m_y);
	}

	///////////////////////////////////////////////////////////////////////////////
	// Return the barycentric weights alpha, beta, and gamma for point p
	///////////////////////////////////////////////////////////////////////////////
	//
	//         (B)
	//         /|\
	//        / | \
	//       /  |  \
	//      /  (P)  \
	//     /  /   \  \
	//    / /       \ \
	//   //           \\
	//  (A)------------(C)
	//
	///////////////////////////////////////////////////////////////////////////////
	template <typename T>
	vector::Vector3d Triangle<T>::get_barycentric_weights(const vector::Vector2d<T>& p) const
	{
		vector::Vector2d<T> a{ m_points[0] };
		vector::Vector2d<T> b{ m_points[1] };
		vector::Vector2d<T> c{ m_points[2] };

		vector::Vector2d<T> ac{ c - a };
		vector::Vector2d<T> ab{ b - a };
		vector::Vector2d<T> ap{ p - a };
		vector::Vector2d<T> pc{ c - p };
		vector::Vector2d<T> pb{ b - p };

		double area_parallelogram_abc{ ac.cross_product(ab) };
		double area_parallelogram_pbc{ pc.cross_product(pb) };

		double alpha{ area_parallelogram_pbc / area_parallelogram_abc };

		double area_parallelogram_apc{ ac.cross_product(ap) };
		double beta{ area_parallelogram_apc / area_parallelogram_abc };

		double gamma{ 1 - alpha - beta };
		return {
			alpha,
			beta,
			gamma
		};
	}
}
