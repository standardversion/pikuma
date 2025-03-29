#pragma once
#include "vector"
#include "vector2d.h"
#include "vector3d.h"
#include "display.h"

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
		//rendering related
		void draw(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t colour) const;
		//flat shaded
		void flat_shade(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour);
		void flat_shade_flat_bottom(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const;
		void flat_shade_flat_top(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const;
		//flat shaded2
		void flat_shade2(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour);
		//gouraud shaded
		void gouraud_shade(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour);
		void gouraud_shade_flat_bottom(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const;
		void gouraud_shade_flat_top(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const;
		//gouraud shaded2
		void gouraud_shade2(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour);


		void shade_pixel(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour, int x, int y) const;
		void shade_pixel2(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour, int x, int y) const;

		std::vector<vector::Vector2d<T>> m_points;
		std::vector<vector::Vector2d<double>> m_uvs;
		std::vector<double> m_points_z;
		std::vector<double> m_points_w;
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
		double point0_z{ m_points_z[0] };
		double point1_z{ m_points_z[1] };
		double point2_z{ m_points_z[2] };
		double point0_w{ m_points_w[0] };
		double point1_w{ m_points_w[1] };
		double point2_w{ m_points_w[2] };
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
			m_points_z[0] = point1_z;
			m_points_z[1] = point0_z;
			m_points_w[0] = point1_w;
			m_points_w[1] = point0_w;
			m_per_vtx_lt_intensity[0] = intensity_p1;
			m_per_vtx_lt_intensity[1] = intensity_p0;
			m_uvs[0] = uv1;
			m_uvs[1] = uv0;
			point0 = m_points[0];
			point1 = m_points[1];
			point2 = m_points[2];
			point0_z = m_points_z[0];
			point1_z = m_points_z[1];
			point2_z = m_points_z[2];
			point0_w = m_points_w[0];
			point1_w = m_points_w[1];
			point2_w = m_points_w[2];
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
			m_points_z[0] = point2_z;
			m_points_z[2] = point0_z;
			m_points_w[0] = point2_w;
			m_points_w[2] = point0_w;
			m_uvs[0] = uv2;
			m_uvs[2] = uv0;
			m_per_vtx_lt_intensity[0] = intensity_p2;
			m_per_vtx_lt_intensity[2] = intensity_p0;
			point0 = m_points[0];
			point1 = m_points[1];
			point2 = m_points[2];
			point0_z = m_points_z[0];
			point1_z = m_points_z[1];
			point2_z = m_points_z[2];
			point0_w = m_points_w[0];
			point1_w = m_points_w[1];
			point2_w = m_points_w[2];
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
			m_points_z[1] = point2_z;
			m_points_z[2] = point1_z;
			m_points_w[1] = point2_w;
			m_points_w[2] = point1_w;
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

	template <typename T>
	void Triangle<T>::draw(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t colour) const
	{
		display::draw_line(
			colour_buffer,
			display_mode,
			m_points[0].m_x,
			m_points[0].m_y,
			m_points[1].m_x,
			m_points[1].m_y,
			colour
		);
		display::draw_line(
			colour_buffer,
			display_mode,
			m_points[0].m_x,
			m_points[0].m_y,
			m_points[2].m_x,
			m_points[2].m_y,
			colour
		);
		display::draw_line(
			colour_buffer,
			display_mode,
			m_points[1].m_x,
			m_points[1].m_y,
			m_points[2].m_x,
			m_points[2].m_y,
			colour
		);
	}

	
	// FLAT SHADING
	// 
	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled triangle with the flat-top/flat-bottom method
	// We split the original triangle in two, half flat-bottom and half flat-top
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
	void Triangle<T>::flat_shade(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour)
	{
		// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
		sort_vertices_by_y();

		if (m_points[1].m_y == m_points[2].m_y)
		{
			flat_shade_flat_bottom(colour_buffer, texture_buffer, display_mode, render_texture, colour);
		}
		else if (m_points[0].m_y == m_points[1].m_y)
		{
			flat_shade_flat_top(colour_buffer, texture_buffer, display_mode, render_texture, colour);
		}
		else {
			vector::Vector2d<int> midpoint{ get_midpoint() };
			vector::Vector3d weights{ get_barycentric_weights(midpoint) };
			double mp_u{ m_uvs[0].m_x * weights.m_x + m_uvs[1].m_x * weights.m_y + m_uvs[2].m_x * weights.m_z };
			double mp_v{ m_uvs[0].m_y * weights.m_x + m_uvs[1].m_y * weights.m_y + m_uvs[2].m_y * weights.m_z };
			geo::Triangle<int> flat_bottom_triangle{ m_points[0], m_points[1], midpoint };
			flat_bottom_triangle.m_light_intensity = m_light_intensity;
			flat_bottom_triangle.m_uvs.push_back(m_uvs[0]);
			flat_bottom_triangle.m_uvs.push_back(m_uvs[1]);
			flat_bottom_triangle.m_uvs.push_back(vector::Vector2d<double>(mp_u, mp_v));
			geo::Triangle<int> flat_top_triangle{ m_points[1], midpoint, m_points[2] };
			flat_top_triangle.m_light_intensity = m_light_intensity;
			flat_top_triangle.m_uvs.push_back(m_uvs[1]);
			flat_top_triangle.m_uvs.push_back(vector::Vector2d<double>(mp_u, mp_v));
			flat_top_triangle.m_uvs.push_back(m_uvs[2]);
			flat_bottom_triangle.flat_shade_flat_bottom(colour_buffer, texture_buffer, display_mode, render_texture, colour);
			flat_top_triangle.flat_shade_flat_top(colour_buffer, texture_buffer, display_mode, render_texture, colour);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled a triangle with a flat bottom
	///////////////////////////////////////////////////////////////////////////////
	//
	//        (x0,y0) a
	//          / \
	//         /   \
	//        /     \
	//       /       \
	//      /         \
	// b (x1,y1)------(x2,y2) c
	//
	///////////////////////////////////////////////////////////////////////////////
	template <typename T>
	void Triangle<T>::flat_shade_flat_bottom(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const
	{
		double x_start_slope{ get_inverse_slope(1, 0) };
		double x_end_slope{ get_inverse_slope(2, 0) };
		double x_start{ static_cast<double>(m_points[0].m_x) };
		double x_end{ static_cast<double>(m_points[0].m_x) };
		vector::Vector2d<double> start_uv{ m_uvs[0] };
		vector::Vector2d<double> end_uv{ m_uvs[0] };
		const std::uint32_t light_colour{ display::apply_light_intensity(colour, m_light_intensity) };
		for (int y{ m_points[0].m_y }; y <= m_points[1].m_y; y++)
		{
			if (render_texture)
			{
				display::draw_line(
					colour_buffer,
					texture_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					start_uv,
					end_uv,
					m_light_intensity
				);
			}
			else
			{
				display::draw_line(
					colour_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					light_colour
				);
			}
			x_start += x_start_slope;
			x_end += x_end_slope;

			vector::Vector3d start_weights{ get_barycentric_weights({ static_cast<int>(x_start), y }) };
			double start_u{ m_uvs[0].m_x * start_weights.m_x + m_uvs[1].m_x * start_weights.m_y + m_uvs[2].m_x * start_weights.m_z };
			double start_v{ m_uvs[0].m_y * start_weights.m_x + m_uvs[1].m_y * start_weights.m_y + m_uvs[2].m_y * start_weights.m_z };
			if (start_u > 1) start_u = 1.0;
			if (start_u < 0) start_u = 0.0;
			if (start_v > 1) start_v = 1.0;
			if (start_v < 0) start_v = 0.0;
			start_uv.m_x = start_u;
			start_uv.m_y = start_v;
			vector::Vector3d end_weights{ get_barycentric_weights({ static_cast<int>(x_end), y }) };
			double end_u{ m_uvs[0].m_x * end_weights.m_x + m_uvs[1].m_x * end_weights.m_y + m_uvs[2].m_x * end_weights.m_z };
			double end_v{ m_uvs[0].m_y * end_weights.m_x + m_uvs[1].m_y * end_weights.m_y + m_uvs[2].m_y * end_weights.m_z };
			if (end_u > 1) end_u = 1.0;
			if (end_u < 0) end_u = 0.0;
			if (end_v > 1) end_v = 1.0;
			if (end_v < 0) end_v = 0.0;
			end_uv.m_x = end_u;
			end_uv.m_y = end_v;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled a triangle with a flat top
	///////////////////////////////////////////////////////////////////////////////
	//
	// a (x0,y0)------(x1,y1) b
	//      \         /
	//       \       /
	//        \     /
	//         \   /
	//          \ /
	//        (x2,y2) c
	//
	///////////////////////////////////////////////////////////////////////////////
	template <typename T>
	void Triangle<T>::flat_shade_flat_top(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const
	{
		double x_start_slope{ get_inverse_slope(2, 0) };
		double x_end_slope{ get_inverse_slope(2, 1) };
		double x_start{ static_cast<double>(m_points[2].m_x) };
		double x_end{ static_cast<double>(m_points[2].m_x) };
		vector::Vector2d<double> start_uv{ m_uvs[2] };
		vector::Vector2d<double> end_uv{ m_uvs[2] };
		const std::uint32_t light_colour{ display::apply_light_intensity(colour, m_light_intensity) };
		for (int y{ m_points[2].m_y }; y >= m_points[0].m_y; y--)
		{
			if (render_texture)
			{
				display::draw_line(
					colour_buffer,
					texture_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					start_uv,
					end_uv,
					m_light_intensity
				);
			}
			else
			{
				display::draw_line(
					colour_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					light_colour
				);
			}
			x_start -= x_start_slope;
			x_end -= x_end_slope;

			vector::Vector3d start_weights{ get_barycentric_weights({ static_cast<int>(x_start), y }) };
			double start_u{ m_uvs[0].m_x * start_weights.m_x + m_uvs[1].m_x * start_weights.m_y + m_uvs[2].m_x * start_weights.m_z };
			double start_v{ m_uvs[0].m_y * start_weights.m_x + m_uvs[1].m_y * start_weights.m_y + m_uvs[2].m_y * start_weights.m_z };
			if (start_u > 1) start_u = 1.0;
			if (start_u < 0) start_u = 0.0;
			if (start_v > 1) start_v = 1.0;
			if (start_v < 0) start_v = 0.0;
			start_uv.m_x = start_u;
			start_uv.m_y = start_v;
			vector::Vector3d end_weights{ get_barycentric_weights({ static_cast<int>(x_end), y }) };
			double end_u{ m_uvs[0].m_x * end_weights.m_x + m_uvs[1].m_x * end_weights.m_y + m_uvs[2].m_x * end_weights.m_z };
			double end_v{ m_uvs[0].m_y * end_weights.m_x + m_uvs[1].m_y * end_weights.m_y + m_uvs[2].m_y * end_weights.m_z };
			if (end_u > 1) end_u = 1.0;
			if (end_u < 0) end_u = 0.0;
			if (end_v > 1) end_v = 1.0;
			if (end_v < 0) end_v = 0.0;
			end_uv.m_x = end_u;
			end_uv.m_y = end_v;
		}
	}

	// FLAT SHADING II
	// 
	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled triangle with the flat-top/flat-bottom method
	// We split the original triangle in two, half flat-bottom and half flat-top
	///////////////////////////////////////////////////////////////////////////////
	//
	//          (x0,y0)
	//            / \
	//           /   \
	//          /     \
	//         /       \
	//        /         \
	//   (x1,y1)---------\
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
	void Triangle<T>::flat_shade2(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour)
	{
		// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
		sort_vertices_by_y();

		const std::uint32_t light_colour{ display::apply_light_intensity(colour, m_light_intensity) };
		double x_start_slope{ 0 };
		double x_end_slope{ 0 };
		///////////////////////////////////////////////////////
		// Render the upper part of the triangle (flat-bottom)
		///////////////////////////////////////////////////////
		if (m_points[1].m_y - m_points[0].m_y != 0) { x_start_slope = get_inverse_slope(1, 0); }
		if (m_points[2].m_y - m_points[0].m_y != 0) { x_end_slope = get_inverse_slope(2, 0); }

		if (m_points[1].m_y - m_points[0].m_y != 0)
		{
			for (int y{ m_points[0].m_y }; y <= m_points[1].m_y; y++)
			{
				int x_start{ static_cast<int>(m_points[1].m_x + (y - m_points[1].m_y) * x_start_slope) };
				int x_end{ static_cast<int>(m_points[0].m_x + (y - m_points[0].m_y) * x_end_slope) };

				if (x_end < x_start)
				{
					int x_tmp = x_end;
					x_end = x_start;
					x_start = x_tmp;
				}
				for (int x{ x_start }; x < x_end; x++)
				{
					if (render_texture)
					{
						shade_pixel(
							colour_buffer,
							texture_buffer,
							display_mode,
							render_texture,
							colour,
							x,
							y
						);
					}
					else
					{
						display::draw_pixel(
							colour_buffer,
							display_mode,
							x,
							y,
							light_colour
						);
					}
				}
			}
		}

		///////////////////////////////////////////////////////
		// Render the bottom part of the triangle (flat-top)
		///////////////////////////////////////////////////////
		x_start_slope = 0;
		x_end_slope = 0;
		if (m_points[2].m_y - m_points[1].m_y != 0) { x_start_slope = get_inverse_slope(2, 1); }
		if (m_points[2].m_y - m_points[0].m_y != 0) { x_end_slope = get_inverse_slope(2, 0); }

		if (m_points[2].m_y - m_points[1].m_y != 0)
		{
			for (int y{ m_points[1].m_y }; y <= m_points[2].m_y; y++)
			{
				int x_start{ static_cast<int>(m_points[1].m_x + (y - m_points[1].m_y) * x_start_slope) };
				int x_end{ static_cast<int>(m_points[0].m_x + (y - m_points[0].m_y) * x_end_slope) };

				if (x_end < x_start)
				{
					int x_tmp = x_end;
					x_end = x_start;
					x_start = x_tmp;
				}
				for (int x{ x_start }; x < x_end; x++)
				{
					if (render_texture)
					{
						shade_pixel(
							colour_buffer,
							texture_buffer,
							display_mode,
							render_texture,
							colour,
							x,
							y
						);
					}
					else
					{
						display::draw_pixel(
							colour_buffer,
							display_mode,
							x,
							y,
							light_colour
						);
					}
				}
			}
		}
	}

	template <typename T>
	void Triangle<T>::shade_pixel(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour, int x, int y) const
	{
		vector::Vector3d weights{ get_barycentric_weights({x, y}) };
		double u{ (m_uvs[0].m_x / m_points_w[0]) * weights.m_x + (m_uvs[1].m_x / m_points_w[1] * weights.m_y) + (m_uvs[2].m_x / m_points_w[2] * weights.m_z) };
		double v{ (m_uvs[0].m_y / m_points_w[0]) * weights.m_x + (m_uvs[1].m_y / m_points_w[1] * weights.m_y) + (m_uvs[2].m_y / m_points_w[2] * weights.m_z) };
		double reciprocal_w{ (1 / m_points_w[0]) * weights.m_x + (1 / m_points_w[1]) * weights.m_y + (1 / m_points_w[2]) * weights.m_z };

		u /= reciprocal_w;
		v /= reciprocal_w;

		int tex_x = abs((int)(u * 64));
		int tex_y = abs((int)(v * 64));
		std::uint32_t texture_colour{ texture_buffer[(64 * tex_y) + tex_x] };
		texture_colour = display::apply_light_intensity(texture_colour, m_light_intensity);
		display::draw_pixel(colour_buffer, display_mode, x, y, texture_colour);
	}



	// GOURAUD SHADING II
	// 
	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled triangle with the flat-top/flat-bottom method
	// We split the original triangle in two, half flat-bottom and half flat-top
	///////////////////////////////////////////////////////////////////////////////
	//
	//          (x0,y0)
	//            / \
	//           /   \
	//          /     \
	//         /       \
	//        /         \
	//   (x1,y1)---------\
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
	void Triangle<T>::gouraud_shade2(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour)
	{
		// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
		sort_vertices_by_y();

		const std::uint32_t light_colour{ display::apply_light_intensity(colour, m_light_intensity) };
		double x_start_slope{ 0 };
		double x_end_slope{ 0 };
		///////////////////////////////////////////////////////
		// Render the upper part of the triangle (flat-bottom)
		///////////////////////////////////////////////////////
		if (m_points[1].m_y - m_points[0].m_y != 0) { x_start_slope = get_inverse_slope(1, 0); }
		if (m_points[2].m_y - m_points[0].m_y != 0) { x_end_slope = get_inverse_slope(2, 0); }

		if (m_points[1].m_y - m_points[0].m_y != 0)
		{
			for (int y{ m_points[0].m_y }; y <= m_points[1].m_y; y++)
			{
				int x_start{ static_cast<int>(m_points[1].m_x + (y - m_points[1].m_y) * x_start_slope) };
				int x_end{ static_cast<int>(m_points[0].m_x + (y - m_points[0].m_y) * x_end_slope) };

				if (x_end < x_start)
				{
					int x_tmp = x_end;
					x_end = x_start;
					x_start = x_tmp;
				}
				for (int x{ x_start }; x < x_end; x++)
				{
					if (render_texture)
					{
						shade_pixel2(
							colour_buffer,
							texture_buffer,
							display_mode,
							render_texture,
							colour,
							x,
							y
						);
					}
					else
					{
						display::draw_pixel(
							colour_buffer,
							display_mode,
							x,
							y,
							light_colour
						);
					}
				}
			}
		}

		///////////////////////////////////////////////////////
		// Render the bottom part of the triangle (flat-top)
		///////////////////////////////////////////////////////
		x_start_slope = 0;
		x_end_slope = 0;
		if (m_points[2].m_y - m_points[1].m_y != 0) { x_start_slope = get_inverse_slope(2, 1); }
		if (m_points[2].m_y - m_points[0].m_y != 0) { x_end_slope = get_inverse_slope(2, 0); }

		if (m_points[2].m_y - m_points[1].m_y != 0)
		{
			for (int y{ m_points[1].m_y }; y <= m_points[2].m_y; y++)
			{
				int x_start{ static_cast<int>(m_points[1].m_x + (y - m_points[1].m_y) * x_start_slope) };
				int x_end{ static_cast<int>(m_points[0].m_x + (y - m_points[0].m_y) * x_end_slope) };

				if (x_end < x_start)
				{
					int x_tmp = x_end;
					x_end = x_start;
					x_start = x_tmp;
				}
				for (int x{ x_start }; x < x_end; x++)
				{
					if (render_texture)
					{
						shade_pixel2(
							colour_buffer,
							texture_buffer,
							display_mode,
							render_texture,
							colour,
							x,
							y
						);
					}
					else
					{
						display::draw_pixel(
							colour_buffer,
							display_mode,
							x,
							y,
							light_colour
						);
					}
				}
			}
		}
	}

	template <typename T>
	void Triangle<T>::shade_pixel2(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour, int x, int y) const
	{
		vector::Vector3d weights{ get_barycentric_weights({x, y}) };
		double u{ (m_uvs[0].m_x / m_points_w[0]) * weights.m_x + (m_uvs[1].m_x / m_points_w[1] * weights.m_y) + (m_uvs[2].m_x / m_points_w[2] * weights.m_z) };
		double v{ (m_uvs[0].m_y / m_points_w[0]) * weights.m_x + (m_uvs[1].m_y / m_points_w[1] * weights.m_y) + (m_uvs[2].m_y / m_points_w[2] * weights.m_z) };
		double reciprocal_w{ (1 / m_points_w[0]) * weights.m_x + (1 / m_points_w[1]) * weights.m_y + (1 / m_points_w[2]) * weights.m_z };

		u /= reciprocal_w;
		v /= reciprocal_w;

		int tex_x = abs((int)(u * 64));
		int tex_y = abs((int)(v * 64));
		std::uint32_t texture_colour{ texture_buffer[(64 * tex_y) + tex_x] };
		double light_intensity{ m_per_vtx_lt_intensity[0] * weights.m_x + m_per_vtx_lt_intensity[1] * weights.m_y + m_per_vtx_lt_intensity[2] * weights.m_z };
		texture_colour = display::apply_light_intensity(texture_colour, light_intensity);
		display::draw_pixel(colour_buffer, display_mode, x, y, texture_colour);
	}

	// GOURAND SHADING
	// 
	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled triangle with the flat-top/flat-bottom method
	// We split the original triangle in two, half flat-bottom and half flat-top
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
	void Triangle<T>::gouraud_shade(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour)
	{
		// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
		sort_vertices_by_y();

		if (m_points[1].m_y == m_points[2].m_y)
		{
			gouraud_shade_flat_bottom(colour_buffer, texture_buffer, display_mode, render_texture, colour);
		}
		else if (m_points[0].m_y == m_points[1].m_y)
		{
			gouraud_shade_flat_top(colour_buffer, texture_buffer, display_mode, render_texture, colour);
		}
		else {
			vector::Vector2d<int> midpoint{ get_midpoint() };
			double scalar_factor{ vector::Vector2d<int>::get_scalar_factor(m_points[0], midpoint, m_points[2]) };
			double mp_intensity{ m_per_vtx_lt_intensity[0] + (m_per_vtx_lt_intensity[2] - m_per_vtx_lt_intensity[0]) * scalar_factor };
			vector::Vector3d weights{ get_barycentric_weights(midpoint) };
			double mp_u{ m_uvs[0].m_x * weights.m_x + m_uvs[1].m_x * weights.m_y + m_uvs[2].m_x * weights.m_z };
			double mp_v{ m_uvs[0].m_y * weights.m_x + m_uvs[1].m_y * weights.m_y + m_uvs[2].m_y * weights.m_z };
			geo::Triangle<int> flat_bottom_triangle{ m_points[0], m_points[1], midpoint };
			flat_bottom_triangle.m_light_intensity = m_light_intensity;
			flat_bottom_triangle.m_per_vtx_lt_intensity.push_back(m_per_vtx_lt_intensity[0]);
			flat_bottom_triangle.m_per_vtx_lt_intensity.push_back(m_per_vtx_lt_intensity[1]);
			flat_bottom_triangle.m_per_vtx_lt_intensity.push_back(mp_intensity);
			flat_bottom_triangle.m_uvs.push_back(m_uvs[0]);
			flat_bottom_triangle.m_uvs.push_back(m_uvs[1]);
			flat_bottom_triangle.m_uvs.push_back(vector::Vector2d<double>(mp_u, mp_v));
			geo::Triangle<int> flat_top_triangle{ m_points[1], midpoint, m_points[2] };
			flat_top_triangle.m_light_intensity = m_light_intensity;
			flat_top_triangle.m_per_vtx_lt_intensity.push_back(m_per_vtx_lt_intensity[1]);
			flat_top_triangle.m_per_vtx_lt_intensity.push_back(mp_intensity);
			flat_top_triangle.m_per_vtx_lt_intensity.push_back(m_per_vtx_lt_intensity[2]);
			flat_top_triangle.m_uvs.push_back(m_uvs[1]);
			flat_top_triangle.m_uvs.push_back(vector::Vector2d<double>(mp_u, mp_v));
			flat_top_triangle.m_uvs.push_back(m_uvs[2]);
			flat_bottom_triangle.gouraud_shade_flat_bottom(colour_buffer, texture_buffer, display_mode, render_texture, colour);
			flat_top_triangle.gouraud_shade_flat_top(colour_buffer, texture_buffer, display_mode, render_texture, colour);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled a triangle with a flat bottom
	///////////////////////////////////////////////////////////////////////////////
	//
	//        (x0,y0) a
	//          / \
	//         /   \
	//        /     \
	//       /       \
	//      /         \
	// b (x1,y1)------(x2,y2) c
	//
	///////////////////////////////////////////////////////////////////////////////
	template <typename T>
	void Triangle<T>::gouraud_shade_flat_bottom(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const
	{
		double x_start_slope{ get_inverse_slope(1, 0) };
		double x_end_slope{ get_inverse_slope(2, 0) };
		double x_start{ static_cast<double>(m_points[0].m_x) };
		double x_end{ static_cast<double>(m_points[0].m_x) };
		double start_intensity{ m_per_vtx_lt_intensity[0] };
		double end_intensity{ m_per_vtx_lt_intensity[0] };
		vector::Vector2d<double> start_uv{ m_uvs[0] };
		vector::Vector2d<double> end_uv{ m_uvs[0] };
		double scalar_factor_ab{ 0 };
		double scalar_factor_ac{ 0 };
		const std::uint32_t light_colour{ display::apply_light_intensity(colour, m_light_intensity) };
		for (int y{ m_points[0].m_y }; y <= m_points[1].m_y; y++)
		{
			if (render_texture)
			{
				display::draw_line(
					colour_buffer,
					texture_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					start_intensity,
					end_intensity,
					start_uv,
					end_uv
				);
			}
			else
			{
				display::draw_line(
					colour_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					start_intensity,
					end_intensity,
					colour
				);
			}
			x_start += x_start_slope;
			x_end += x_end_slope;
			/*
			For two values A and B, and a scalar factor λ, the interpolated value I is given by:

							I=A+λ⋅(B−A)
			Interpolate the intensity at each start and end pixels
			*/
			scalar_factor_ab = vector::Vector2d<int>::get_scalar_factor(m_points[0], { static_cast<int>(x_start), y }, m_points[1]);
			start_intensity = m_per_vtx_lt_intensity[0] + (m_per_vtx_lt_intensity[1] - m_per_vtx_lt_intensity[0]) * scalar_factor_ab;
			scalar_factor_ac = vector::Vector2d<int>::get_scalar_factor(m_points[0], { static_cast<int>(x_end), y }, m_points[2]);
			end_intensity = m_per_vtx_lt_intensity[0] + (m_per_vtx_lt_intensity[2] - m_per_vtx_lt_intensity[0]) * scalar_factor_ab;

			vector::Vector3d start_weights{ get_barycentric_weights({ static_cast<int>(x_start), y }) };
			double start_u{ m_uvs[0].m_x * start_weights.m_x + m_uvs[1].m_x * start_weights.m_y + m_uvs[2].m_x * start_weights.m_z };
			double start_v{ m_uvs[0].m_y * start_weights.m_x + m_uvs[1].m_y * start_weights.m_y + m_uvs[2].m_y * start_weights.m_z };
			if (start_u > 1) start_u = 1.0;
			if (start_u < 0) start_u = 0.0;
			if (start_v > 1) start_v = 1.0;
			if (start_v < 0) start_v = 0.0;
			start_uv.m_x = start_u;
			start_uv.m_y = start_v;
			vector::Vector3d end_weights{ get_barycentric_weights({ static_cast<int>(x_end), y }) };
			double end_u{ m_uvs[0].m_x * end_weights.m_x + m_uvs[1].m_x * end_weights.m_y + m_uvs[2].m_x * end_weights.m_z };
			double end_v{ m_uvs[0].m_y * end_weights.m_x + m_uvs[1].m_y * end_weights.m_y + m_uvs[2].m_y * end_weights.m_z };
			if (end_u > 1) end_u = 1.0;
			if (end_u < 0) end_u = 0.0;
			if (end_v > 1) end_v = 1.0;
			if (end_v < 0) end_v = 0.0;
			end_uv.m_x = end_u;
			end_uv.m_y = end_v;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled a triangle with a flat top
	///////////////////////////////////////////////////////////////////////////////
	//
	// a (x0,y0)------(x1,y1) b
	//      \         /
	//       \       /
	//        \     /
	//         \   /
	//          \ /
	//        (x2,y2) c
	//
	///////////////////////////////////////////////////////////////////////////////
	template <typename T>
	void Triangle<T>::gouraud_shade_flat_top(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, const bool render_texture, const std::uint32_t colour) const
	{
		double x_start_slope{ get_inverse_slope(2, 0) };
		double x_end_slope{ get_inverse_slope(2, 1) };
		double x_start{ static_cast<double>(m_points[2].m_x) };
		double x_end{ static_cast<double>(m_points[2].m_x) };
		double start_intensity{ m_per_vtx_lt_intensity[2] };
		double end_intensity{ m_per_vtx_lt_intensity[2] };
		vector::Vector2d<double> start_uv{ m_uvs[2] };
		vector::Vector2d<double> end_uv{ m_uvs[2] };
		double scalar_factor_ac{ 0 };
		double scalar_factor_bc{ 0 };
		const std::uint32_t light_colour{ display::apply_light_intensity(colour, m_light_intensity) };
		for (int y{ m_points[2].m_y }; y >= m_points[0].m_y; y--)
		{
			if (render_texture)
			{
				display::draw_line(
					colour_buffer,
					texture_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					start_intensity,
					end_intensity,
					start_uv,
					end_uv
				);
			}
			else
			{
				display::draw_line(
					colour_buffer,
					display_mode,
					x_start,
					y,
					x_end,
					y,
					start_intensity,
					end_intensity,
					colour
				);
			}
			x_start -= x_start_slope;
			x_end -= x_end_slope;
			/*
			For two values A and B, and a scalar factor λ, the interpolated value I is given by:

							I=A+λ⋅(B−A)
			Interpolate the intensity at each start and end pixels
			*/
			scalar_factor_ac = vector::Vector2d<int>::get_scalar_factor(m_points[0], { static_cast<int>(x_start), static_cast<int>(y) }, m_points[2]);
			start_intensity = m_per_vtx_lt_intensity[0] + (m_per_vtx_lt_intensity[2] - m_per_vtx_lt_intensity[0]) * scalar_factor_ac;
			scalar_factor_bc = vector::Vector2d<int>::get_scalar_factor(m_points[1], { static_cast<int>(x_end), static_cast<int>(y) }, m_points[2]);
			end_intensity = m_per_vtx_lt_intensity[1] + (m_per_vtx_lt_intensity[2] - m_per_vtx_lt_intensity[1]) * scalar_factor_bc;

			vector::Vector3d start_weights{ get_barycentric_weights({ static_cast<int>(x_start), y }) };
			double start_u{ m_uvs[0].m_x * start_weights.m_x + m_uvs[1].m_x * start_weights.m_y + m_uvs[2].m_x * start_weights.m_z };
			double start_v{ m_uvs[0].m_y * start_weights.m_x + m_uvs[1].m_y * start_weights.m_y + m_uvs[2].m_y * start_weights.m_z };
			if (start_u > 1) start_u = 1.0;
			if (start_u < 0) start_u = 0.0;
			if (start_v > 1) start_v = 1.0;
			if (start_v < 0) start_v = 0.0;
			start_uv.m_x = start_u;
			start_uv.m_y = start_v;
			vector::Vector3d end_weights{ get_barycentric_weights({ static_cast<int>(x_end), y }) };
			double end_u{ m_uvs[0].m_x * end_weights.m_x + m_uvs[1].m_x * end_weights.m_y + m_uvs[2].m_x * end_weights.m_z };
			double end_v{ m_uvs[0].m_y * end_weights.m_x + m_uvs[1].m_y * end_weights.m_y + m_uvs[2].m_y * end_weights.m_z };
			if (end_u > 1) end_u = 1.0;
			if (end_u < 0) end_u = 0.0;
			if (end_v > 1) end_v = 1.0;
			if (end_v < 0) end_v = 0.0;
			end_uv.m_x = end_u;
			end_uv.m_y = end_v;
		}
	}
}
