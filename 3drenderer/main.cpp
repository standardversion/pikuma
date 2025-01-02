#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include "display.h"
#include "input.h"
#include "sdlwrapper.h"
#include "vector3d.h"
#include "vector2d.h"

void update(
	std::vector<vector::Vector3d>& cube_points,
	std::vector<vector::Vector2d>& projected_points,
	double fov_factor,
	vector::Vector3d& camera_position,
	vector::Vector3d& cube_rotation
)
{
	projected_points = {};
	double cam_z{ camera_position.get_z() };
	cube_rotation.set_x(cube_rotation.get_x() + 0.01);
	cube_rotation.set_y(cube_rotation.get_y() + 0.01);
	cube_rotation.set_z(cube_rotation.get_z() + 0.01);
	for (auto& cube_point : cube_points)
	{
		//Create copy of the point since we do not want to apply transforms to the original cube points
		//we'll project this transformed point
		vector::Vector3d point{ cube_point };
		//rotate the point
		point.rotate_x(cube_rotation.get_x());
		point.rotate_y(cube_rotation.get_y());
		point.rotate_z(cube_rotation.get_z());
		// move pionts away from camera
		point.set_z(point.get_z() - camera_position.get_z());

		projected_points.push_back(point.project(fov_factor));
	}
}


int main(int argc, char* argv[])
{
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* colour_buffer_texture{ nullptr };
	SDL_DisplayMode display_mode;
	SDLWrapper sdl{};
	display::Display display{};
	input::Input input{};
	bool is_running{ display.setup(colour_buffer_texture, window, renderer, &display_mode, sdl) };
	std::uint32_t* colour_buffer{ new std::uint32_t[display_mode.w * display_mode.h]{} };
	std::uint32_t bg_colour{ 0x00000000 };
	std::uint32_t line_colour{ 0xFFFFFFFF };
	std::uint32_t rect_colour{ 0xFFFFFF00 };
	std::vector<vector::Vector3d> cube_points{};
	for (double x{ -1.0 }; x <= 1.0; x += .25)
	{
		for (double y{ -1.0 }; y <= 1.0; y += .25)
		{
			for (double z{ -1.0 }; z <= 1.0; z += .25)
			{
				cube_points.push_back(vector::Vector3d{ x, y, z });
			}
		}
	}
	std::vector<vector::Vector2d> projected_points{};
	SDL_Event event{};
	double fov_factor{ 640.0 };
	vector::Vector3d camera_postion{ 0.0, 0.0, -5.0 };
	vector::Vector3d cube_rotation{ 0.0, 0.0, 0.0 };
	while (is_running)
	{
		input.process(is_running, event, sdl);
		update(cube_points, projected_points, fov_factor, camera_postion, cube_rotation);
		display.render(
			renderer,
			colour_buffer_texture,
			colour_buffer,
			&display_mode,
			line_colour,
			bg_colour,
			10,
			projected_points,
			sdl
		);
	}
	display.cleanup(window, renderer, colour_buffer, sdl);
	return 0;
}