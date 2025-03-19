#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include "vector2d.h"
#include "vector4d.h"
#include "triangle.h"
#include "matrix.h"


//https://wiki.libsdl.org/SDL2/CategoryAPI

namespace display
{
	inline int FPS{ 30 };
	inline double FRAME_TARGET_TIME(1000 / FPS);
	enum RenderModes {
		wireframe, //0
		wireframe_vertex, //1
		wireframe_vertex_face_center, //2
		wireframe_vertex_face_center_normals, //3
		shaded, //4
		shaded_wireframe, //5
		shaded_wireframe_vertex, //6
		shaded_wireframe_vertex_face_center, //7
		shaded_wireframe_vertex_face_center_normals, //8
		total_mode // count
	};
	class Display
	{
	public:
		virtual ~Display() = default;
		virtual void activate_render_mode(const int render_mode, bool& render_wireframe, bool& render_vertex, bool& render_shaded, bool& render_face_center, bool& render_normals) const;
		virtual std::uint32_t apply_light_intensity(const std::uint32_t color, double percentage_factor) const;
		virtual void cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer) const;
		virtual void clear_colour_buffer(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t colour) const;
		virtual void draw_grid(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t line_colour, const std::uint32_t bg_colour, const int grid_on) const;
		virtual void draw_line(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const std::uint32_t colour) const;
		virtual void draw_line(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, double start_i, double end_i, const std::uint32_t colour) const;
		virtual void draw_pixel(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x, int y, const std::uint32_t colour) const;
		virtual void draw_rect(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int start_x, int start_y, int width, int height, const std::uint32_t colour) const;
		virtual void draw_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour) const;
		virtual void fill_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gourand_shaded, geo::Triangle<int>& triangle, const std::uint32_t colour) const;
		virtual void fill_flat_bottom_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gourand_shaded, const geo::Triangle<int>& triangle, const std::uint32_t colour) const;
		virtual void fill_flat_top_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gourand_shaded, const geo::Triangle<int>& triangle, const std::uint32_t colour) const;
		virtual bool initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode) const;
		virtual vector::Vector2d<double> project_vec4d(const SDL_DisplayMode* display_mode, const matrix::Matrix4x4& projection_matrix, const vector::Vector4d& vec4d) const;
		virtual void render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, SDL_Renderer*& renderer) const;
		virtual bool setup(SDL_Texture*& colour_buffer_texture, SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* displaymode) const;
	};
}

