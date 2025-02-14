#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include "display.h"
#include "input.h"
#include "light.h"
#include "sdlwrapper.h"
#include "vector3d.h"
#include "vector2d.h"
#include "mesh.h"
#include "triangle.h"
#include "utils.h"

void update(
	geo::Mesh& mesh_to_render,
	std::vector<geo::Triangle<int>>& triangles_to_render,
	const double fov_factor,
	const vector::Vector3d& camera_position,
	const SDL_DisplayMode* display_mode,
	int& previous_frame_time,
	const bool backface_culling,
	const shading::Light& light,
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

	mesh_to_render.m_rotation.m_x += 0.01;
	mesh_to_render.m_rotation.m_y += 0.01;
	mesh_to_render.m_rotation.m_z += 0.01;
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
		// convert the triangle points to ints as we'll project to screen space so we're dealing with x & y pixels which are in ints
		geo::Triangle<int> projected_triangle{ };
		std::vector<vector::Vector3d> transformed_vertices{};
		// apply transformations
		for (const auto& vertex : face_vertices)
		{
			vector::Vector3d point{ vertex };
			//rotate the point
			point.rotate_x(mesh_to_render.m_rotation.m_x);
			point.rotate_y(mesh_to_render.m_rotation.m_y);
			point.rotate_z(mesh_to_render.m_rotation.m_z);
			// move pionts away from camera
			point.m_z += 5.0;
			transformed_vertices.push_back(point);
		}
		// get avg depth of each face so we can sort and render by depth
		projected_triangle.m_avg_depth = (transformed_vertices[0].m_z + transformed_vertices[1].m_z + transformed_vertices[2].m_z) / 3.0;
		 
		if (backface_culling) {
			// check backface culling
			vector::Vector3d vector_a{ transformed_vertices[0] };  /*   A   */
			vector::Vector3d vector_b{ transformed_vertices[1] };  /*  / \  */
			vector::Vector3d vector_c{ transformed_vertices[2] };  /* C---B */
			// get vector subtraction of B-A and C-A
			vector::Vector3d vector_ab{ vector_b - vector_a };
			vector::Vector3d vector_ac{ vector_c - vector_a };
			vector_ab.normalize();
			vector_ac.normalize();
			// compute the normal at the vertex (using cross product to find perpendicular)
			// order of the cross product depends on the coordinate system
			// since this is a left handed system our order of cross product is ab x ac
			vector::Vector3d normal{ vector_ab.cross_product(vector_ac) };
			normal.normalize();
			// find the camera ray ie the vector between a point in the triangle and the camera origin
			vector::Vector3d camera_ray{ camera_position - vector_a };

			// calculate how aligned the camera ray is with the face normal (using dot product)
			if (camera_ray.dot_product(normal) < 0)
			{
				continue;
			}
		}
		vector::Vector3d face_center{ mesh_to_render.get_face_center(transformed_vertices) };
		vector::Vector3d face_normal{ mesh_to_render.get_face_normal(face_center, transformed_vertices) };
		face_normal.normalize();
		double light_intensity{ abs(face_normal.dot_product(light.m_direction)) };
		projected_triangle.m_light_intensity = light_intensity;
		// project the point
		std::size_t counter{ 0 };
		for (const auto& vertex : transformed_vertices)
		{
			vector::Vector2d<double> projected_point{ vertex.project(fov_factor) };
			projected_point.m_x += display_mode->w / 2;
			projected_point.m_y += display_mode->h / 2;
			projected_triangle.m_points.push_back(vector::Vector2d<int>{static_cast<int>(projected_point.m_x), static_cast<int>(projected_point.m_y)});
		}
		vector::Vector2d<double> projected_center{ face_center.project(fov_factor) };
		projected_center.m_x += display_mode->w / 2;
		projected_center.m_y += display_mode->h / 2;
		/*projected_triangle.m_center.m_x = static_cast<int>(projected_center.m_x);
		projected_triangle.m_center.m_y = static_cast<int>(projected_center.m_y);*/
		projected_triangle.m_center.m_x = projected_center.m_x;
		projected_triangle.m_center.m_y = projected_center.m_y;

		vector::Vector2d<double> projected_normal{ face_normal.project(fov_factor) };
		projected_normal.m_x += display_mode->w / 2;
		projected_normal.m_y += display_mode->h / 2;
		/*projected_triangle.m_face_normal.m_x = static_cast<int>(projected_normal.m_x);
		projected_triangle.m_face_normal.m_y = static_cast<int>(projected_normal.m_y);*/
		projected_triangle.m_face_normal.m_x = projected_normal.m_x;
		projected_triangle.m_face_normal.m_y = projected_normal.m_y;
		triangles_to_render.push_back(projected_triangle);
	}
	std::sort(triangles_to_render.begin(), triangles_to_render.end(), [](const geo::Triangle<int>& a, const geo::Triangle<int>& b)
		{
			return a.m_avg_depth > b.m_avg_depth;
		}
	);
}

void render(
	const display::Display& display,
	SDL_Renderer*& renderer,
	SDL_Texture*& colour_buffer_texture,
	std::uint32_t*& colour_buffer,
	const SDL_DisplayMode* display_mode,
	const std::uint32_t edge_colour,
	const std::uint32_t bg_colour,
	const std::uint32_t vertex_colour,
	const std::uint32_t fill_colour,
	const int grid_on,
	std::vector<geo::Triangle<int>>& triangles_to_render,
	const int render_mode,
	const SDLWrapper& sdl
)
{
	bool render_wireframe{ false };
	bool render_vertex{ false };
	bool render_shaded{ false };
	bool render_face_center{ false };
	bool render_normals{ false };
	display.activate_render_mode(render_mode, render_wireframe, render_vertex, render_shaded, render_face_center, render_normals);
	//display.draw_grid(colour_buffer, display_mode, edge_colour, bg_colour, grid_on);

	for (auto& triangle : triangles_to_render)
	{
		if (render_shaded)
		{
			display.fill_triangle(
				colour_buffer,
				display_mode,
				triangle,
				fill_colour
			);
		}
		
		if (render_wireframe)
		{
			display.draw_triangle(
				colour_buffer,
				display_mode,
				triangle,
				edge_colour
			);
		}
		
		if (render_vertex)
		{
			for (const auto& point : triangle.m_points)
			{
				display.draw_rect(
					colour_buffer,
					display_mode,
					point.m_x,
					point.m_y,
					4,
					4,
					vertex_colour
				);
			}
		}
		if (render_face_center)
		{
			display.draw_rect(
				colour_buffer,
				display_mode,
				triangle.m_center.m_x,
				triangle.m_center.m_y,
				4,
				4,
				vertex_colour
			);
		}
		if (render_normals)
		{
			display.draw_line(
				colour_buffer,
				display_mode,
				triangle.m_center.m_x,
				triangle.m_center.m_y,
				triangle.m_face_normal.m_x,
				triangle.m_face_normal.m_y,
				edge_colour
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
	constexpr const std::uint32_t edge_colour{ 0xFFFFFFFF };
	constexpr const std::uint32_t vertex_colour{ 0xFFFFFF00 };
	constexpr const std::uint32_t fill_colour{ 0xFF0000FF };
	const shading::Light directional_light{};

	std::vector<geo::Triangle<int>> triangles_to_render{};
	SDL_Event event{};
	constexpr const double fov_factor{ 640.0 };
	const vector::Vector3d camera_postion{ 0.0, 0.0, 0.0 };
	int previous_frame_time{ 0 };
	geo::Mesh mesh{ ".\\assets\\sphere.obj" };
	int render_mode{ display::RenderModes::shaded };
	bool backface_culling{ true };
	while (is_running)
	{
		input.process(is_running, render_mode, backface_culling, event, sdl);
		update(mesh, triangles_to_render, fov_factor, camera_postion, &display_mode, previous_frame_time, backface_culling, directional_light, sdl);
		render(
			display,
			renderer,
			colour_buffer_texture,
			colour_buffer,
			&display_mode,
			edge_colour,
			bg_colour,
			vertex_colour,
			fill_colour,
			10,
			triangles_to_render,
			render_mode,
			sdl
		);
	}
	display.cleanup(window, renderer, colour_buffer, sdl);
	return 0;
}