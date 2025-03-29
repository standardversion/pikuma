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
		//shade the triangle
		void fill(std::uint32_t*& colour_buffer, const SDL_Surface* surface, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gouraud_shaded, const bool render_texture, const std::uint32_t colour);
		void fill_pixel(std::uint32_t*& colour_buffer, const SDL_Surface* surface, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gouraud_shaded, const bool render_texture, const std::uint32_t colour, int x, int y) const;

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

	// SHADING
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
	void Triangle<T>::fill(std::uint32_t*& colour_buffer, const SDL_Surface* surface, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gouraud_shaded, const bool render_texture, const std::uint32_t colour)
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
					fill_pixel(
						colour_buffer,
						surface,
						display_mode,
						render_flat_shaded,
						render_gouraud_shaded,
						render_texture,
						colour,
						x,
						y
					);
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
					fill_pixel(
						colour_buffer,
						surface,
						display_mode,
						render_flat_shaded,
						render_gouraud_shaded,
						render_texture,
						colour,
						x,
						y
					);
				}
			}
		}
	}

	template <typename T>
	void Triangle<T>::fill_pixel(std::uint32_t*& colour_buffer, const SDL_Surface* surface, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gouraud_shaded, const bool render_texture, const std::uint32_t colour, int x, int y) const
	{
		vector::Vector3d weights{ get_barycentric_weights({x, y}) };
		double light_intensity{ 1.0 };
		std::uint32_t pixel_colour{ colour };
		if (render_flat_shaded)
		{
			light_intensity = m_light_intensity;
		}
		if (render_gouraud_shaded)
		{
			light_intensity = m_per_vtx_lt_intensity[0] * weights.m_x + m_per_vtx_lt_intensity[1] * weights.m_y + m_per_vtx_lt_intensity[2] * weights.m_z;
			
		}
		if (render_texture)
		{
			double u{ (m_uvs[0].m_x / m_points_w[0]) * weights.m_x + (m_uvs[1].m_x / m_points_w[1] * weights.m_y) + (m_uvs[2].m_x / m_points_w[2] * weights.m_z) };
			double v{ (m_uvs[0].m_y / m_points_w[0]) * weights.m_x + (m_uvs[1].m_y / m_points_w[1] * weights.m_y) + (m_uvs[2].m_y / m_points_w[2] * weights.m_z) };
			double reciprocal_w{ (1 / m_points_w[0]) * weights.m_x + (1 / m_points_w[1]) * weights.m_y + (1 / m_points_w[2]) * weights.m_z };
			u /= reciprocal_w;
			v /= reciprocal_w;

			int tex_x = abs((int)(u * surface->w));
			int tex_y = abs((int)(v * surface->h));

			pixel_colour = display::get_pixel_colour(surface, tex_x, tex_y);
		}
		pixel_colour = display::apply_light_intensity(pixel_colour, light_intensity);

		display::draw_pixel(colour_buffer, display_mode, x, y, pixel_colour);
	}
}
