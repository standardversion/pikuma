#pragma once
#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include "vector2d.h"
#include "vector4d.h"
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

	void activate_render_mode(const int render_mode, bool& render_wireframe, bool& render_vertex, bool& render_shaded, bool& render_face_center, bool& render_normals);
	std::uint32_t apply_light_intensity(const std::uint32_t color, double percentage_factor);
	void cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer);
	void clear_colour_buffer(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t colour);
	void draw_grid(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t line_colour, const std::uint32_t bg_colour, const int grid_on);
	// draw line for flat shading
	void draw_line(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const std::uint32_t colour);
	// draw line for gouraud shading
	void draw_line(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, double start_i, double end_i, const std::uint32_t colour);
	// draw line for texture with flat shaing
	void draw_line(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const vector::Vector2d<double>& start_uv, const vector::Vector2d<double>& end_uv, double light_intensity);
	// draw line for texture with gouraud shading
	void draw_line(std::uint32_t*& colour_buffer, const std::uint32_t*& texture_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, double start_i, double end_i, const vector::Vector2d<double>& start_uv, const vector::Vector2d<double>& end_uv);
	void draw_pixel(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x, int y, const std::uint32_t colour);
	void draw_rect(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int start_x, int start_y, int width, int height, const std::uint32_t colour);
	bool initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode);
	vector::Vector2d<double> project_vec4d(const SDL_DisplayMode* display_mode, const matrix::Matrix4x4& projection_matrix, const vector::Vector4d& vec4d);
	void render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, SDL_Renderer*& renderer);
	bool setup(SDL_Texture*& colour_buffer_texture, SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* displaymode);
}

