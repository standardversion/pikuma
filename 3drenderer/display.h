#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
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
	static SDL_Window* window{ nullptr };
	static SDL_Renderer* renderer{ nullptr };
	static SDL_Texture* colour_buffer_texture{ nullptr };
	static SDL_DisplayMode display_mode;
	static std::uint32_t* colour_buffer{ nullptr };
	static double* z_buffer{ nullptr };
	static bool make_low_rez{ true };
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
	void cleanup();
	void clear_colour_buffer(const std::uint32_t colour);
	void clear_z_buffer();
	void draw_grid(const std::uint32_t line_colour, const std::uint32_t bg_colour, const int grid_on);
	void draw_line(int x0, int y0, int x1, int y1, const std::uint32_t colour);
	void draw_pixel(int x, int y, const std::uint32_t colour);
	void draw_rect(int start_x, int start_y, int width, int height, const std::uint32_t colour);
	vector::Vector2d<int> get_display_width_height();
	std::uint32_t get_pixel_colour(const SDL_Surface* surface, const int x, const int y);
	double get_z_buffer_value(const std::size_t index);
	void set_z_buffer_value(const std::size_t index, const double value);
	void toggle_make_low_rez();
	bool initialize_window();
	vector::Vector4d project_vec4d(const matrix::Matrix4x4& projection_matrix, const vector::Vector4d& vec4d);
	void render_colour_buffer();
	void render_present();
	bool setup();
}

