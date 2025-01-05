#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include <string>
#include "display.h"
#include "input.h"
#include "sdlwrapper.h"
#include "vector3d.h"
#include "vector2d.h"
#include "mesh.h"
#include "triangle.h"
#include "utils.h"

void update(
	geo::Mesh& mesh_to_render,
	std::vector<geo::triangle_t>& triangles_to_render,
	const double fov_factor,
	const vector::Vector3d& camera_position,
	const SDL_DisplayMode* display_mode,
	int& previous_frame_time,
	const SDLWrapper& sdl
)
{
	int time_to_wait = display::FRAME_TARGET_TIME - (sdl.SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= display::FRAME_TARGET_TIME)
	{
		sdl.SDL_Delay(time_to_wait);
	}

	previous_frame_time = sdl.SDL_GetTicks();

	triangles_to_render = {};

	//mesh_to_render.m_rotation.m_x += 0.01;
	mesh_to_render.m_rotation.m_y += 0.01;
	//mesh_to_render.m_rotation.m_z += 0.01;
	for (const auto& face : mesh_to_render.m_faces)
	{
		//each mesh face has 3 vertices stored as the index of the vertex in mesh_vertices
		//face_vertices is an array of 3 vector3d  containing x,y,z coordinates of each vertex
		std::vector<vector::Vector3d> face_vertices{
			mesh_to_render.m_vertices[face.a - 1],
			mesh_to_render.m_vertices[face.b - 1],
			mesh_to_render.m_vertices[face.c - 1]
		};
		//loop through each vertex and project the points of those faces
		geo::triangle_t projected_triangle{};
		std::size_t counter{ 0 };
		for (const auto& vertex : face_vertices)
		{
			vector::Vector3d point{ vertex };
			//rotate the point
			point.rotate_x(mesh_to_render.m_rotation.m_x);
			point.rotate_y(mesh_to_render.m_rotation.m_y);
			point.rotate_z(mesh_to_render.m_rotation.m_z);
			// move pionts away from camera
			point.m_z -= camera_position.m_z;
			vector::Vector2d projected_point{ point.project(fov_factor) };
			projected_point.m_x += display_mode->w / 2;
			projected_point.m_y += display_mode->h / 2;
			projected_triangle.points[counter++] = projected_point;
		}
		triangles_to_render.push_back(projected_triangle);
	}
}

void render(
	const display::Display& display,
	SDL_Renderer*& renderer,
	SDL_Texture*& colour_buffer_texture,
	std::uint32_t*& colour_buffer,
	const SDL_DisplayMode* display_mode,
	const std::uint32_t line_colour,
	const std::uint32_t bg_colour,
	const int grid_on,
	const std::vector<geo::triangle_t>& triangles_to_render,
	const SDLWrapper& sdl
)
{

	//display.draw_grid(colour_buffer, display_mode, line_colour, bg_colour, grid_on);

	for (const auto& triangle : triangles_to_render)
	{
		
		display.draw_triangle(
			colour_buffer,
			display_mode,
			triangle,
			line_colour
		);

		for (const auto& point : triangle.points)
		{
			display.draw_rect(
				colour_buffer,
				display_mode,
				point.m_x,
				point.m_y,
				2,
				2,
				0xFFFFFF00
			);
		}

	}

	// render the colour buffer
	display.render_colour_buffer(colour_buffer_texture, colour_buffer, display_mode, renderer, sdl);
	// fill the colour buffer with a colour value
	display.clear_colour_buffer(colour_buffer, display_mode, bg_colour);

	// Update the screen with any rendering performed since the previous call.
	sdl.SDL_RenderPresent(renderer);

}


int main(int argc, char* argv[])
{
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* colour_buffer_texture{ nullptr };
	SDL_DisplayMode display_mode;
	const SDLWrapper sdl{};
	display::Display display{};
	const input::Input input{};
	bool is_running{ display.setup(colour_buffer_texture, window, renderer, &display_mode, sdl) };
	std::uint32_t* colour_buffer{ new std::uint32_t[display_mode.w * display_mode.h]{} };
	constexpr const std::uint32_t bg_colour{ 0x00000000 };
	constexpr const std::uint32_t line_colour{ 0xFFFFFFFF };
	constexpr const std::uint32_t rect_colour{ 0xFFFFFF00 };

	std::vector<geo::triangle_t> triangles_to_render{};
	SDL_Event event{};
	constexpr const double fov_factor{ 640.0 };
	const vector::Vector3d camera_postion{ 0.0, 0.0, -5.0 };
	int previous_frame_time{ 0 };
	geo::Mesh mesh(".\\assets\\monkey.obj");
	while (is_running)
	{
		input.process(is_running, event, sdl);
		update(mesh, triangles_to_render, fov_factor, camera_postion, &display_mode, previous_frame_time, sdl);
		render(
			display,
			renderer,
			colour_buffer_texture,
			colour_buffer,
			&display_mode,
			line_colour,
			bg_colour,
			10,
			triangles_to_render,
			sdl
		);
	}
	display.cleanup(window, renderer, colour_buffer, sdl);
	return 0;
}