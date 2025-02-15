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
#include "matrix.h"

void update(
	std::vector<geo::Mesh>& meshes,
	const matrix::Matrix4x4& projection_matrix,
	std::vector<geo::Triangle<int>>& triangles_to_render,
	const vector::Vector3d& camera_position,
	const display::Display& display,
	const SDL_DisplayMode* display_mode,
	int& previous_frame_time,
	const bool backface_culling,
	const shading::Light& light,
	const bool render_face_center,
	const bool render_normals,
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
	int counter{ 0 };
	for (auto& mesh_to_render : meshes)
	{
		if (counter) {
			mesh_to_render.m_scale.m_x = 1.2;
			mesh_to_render.m_scale.m_y = 1.2;
			mesh_to_render.m_scale.m_z = 1.2;
		}
		mesh_to_render.m_rotation.m_x += 0.01;
		mesh_to_render.m_rotation.m_y += 0.01;
		mesh_to_render.m_rotation.m_z += 0.01;
		/*mesh_to_render.m_scale.m_x += 0.01;
		mesh_to_render.m_scale.m_y += 0.01;
		mesh_to_render.m_scale.m_z += 0.01;*/
		//mesh_to_render.m_translation.m_x += 0.01;
		// move pionts away from camera
		mesh_to_render.m_translation.m_z = 5.0;

		matrix::Matrix4x4 scale_matrix{ matrix::Matrix4x4::make_scale_matrix(mesh_to_render.m_scale.m_x, mesh_to_render.m_scale.m_y, mesh_to_render.m_scale.m_z) };
		matrix::Matrix4x4 translation_matrix{ matrix::Matrix4x4::make_translation_matrix(mesh_to_render.m_translation.m_x, mesh_to_render.m_translation.m_y, mesh_to_render.m_translation.m_z) };
		matrix::Matrix4x4 rotatation_matrix_x{ matrix::Matrix4x4::make_rotation_matrix(mesh_to_render.m_rotation.m_x, 'x') };
		matrix::Matrix4x4 rotatation_matrix_y{ matrix::Matrix4x4::make_rotation_matrix(mesh_to_render.m_rotation.m_y, 'y') };
		matrix::Matrix4x4 rotatation_matrix_z{ matrix::Matrix4x4::make_rotation_matrix(mesh_to_render.m_rotation.m_z, 'z') };

		matrix::Matrix4x4 scale_face_center_matrix{ matrix::Matrix4x4::make_scale_matrix(1.1, 1.1, 1.1) };

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
			std::vector<vector::Vector4d> transformed_vertices{};
			// apply transformations
			for (const auto& vertex : face_vertices)
			{
				vector::Vector4d point{ vertex };
				matrix::Matrix4x4 world_matrix{};
				// order matters scale, then rotate and then translate
				world_matrix *= scale_matrix;
				world_matrix *= rotatation_matrix_z;
				world_matrix *= rotatation_matrix_y;
				world_matrix *= rotatation_matrix_x;
				world_matrix *= translation_matrix;
				point = world_matrix.mult_vec4d(point);
				transformed_vertices.push_back(point);
			}
			// get avg depth of each face so we can sort and render by depth
			projected_triangle.m_avg_depth = (transformed_vertices[0].m_z + transformed_vertices[1].m_z + transformed_vertices[2].m_z) / 3.0;
			std::vector<vector::Vector3d> per_vertex_normals{ mesh_to_render.get_per_vertex_normals(transformed_vertices) };
			if (backface_culling) {
				// find the camera ray ie the vector between a point in the triangle and the camera origin
				vector::Vector3d camera_ray{ camera_position - transformed_vertices[0] };

				// calculate how aligned the camera ray is with the face normal (using dot product)
				if (camera_ray.dot_product(per_vertex_normals[0]) < 0)
				{
					continue;
				}
			}
			// calculate light intensity at the face			
			double light_intensity{ abs(per_vertex_normals[0].dot_product(light.m_direction))};
			projected_triangle.m_light_intensity = light_intensity;

			// project the point
			for (const auto& vertex : transformed_vertices)
			{
				vector::Vector2d<double> projected_point{ display.project_vec4d(display_mode, projection_matrix, vertex) };
				projected_triangle.m_points.push_back(vector::Vector2d<int>{static_cast<int>(projected_point.m_x), static_cast<int>(projected_point.m_y)});
			}
			if (render_face_center || render_normals)
			{
				vector::Vector3d face_center{ mesh_to_render.get_face_center(transformed_vertices) };
				vector::Vector2d<double> projected_center_point{ display.project_vec4d(display_mode, projection_matrix, face_center) };

				projected_triangle.m_center.m_x = projected_center_point.m_x;
				projected_triangle.m_center.m_y = projected_center_point.m_y;
				if (render_normals)
				{
					vector::Vector3d face_normal{ per_vertex_normals[0] };
					// move the normal to the center of the face
					face_normal.m_x += face_center.m_x;
					face_normal.m_y += face_center.m_y;
					face_normal.m_z += face_center.m_z;
					face_normal.normalize();
			
					vector::Vector2d<double> projected_face_normal_point{ display.project_vec4d(display_mode, projection_matrix, face_normal) };

					projected_triangle.m_face_normal.m_x = projected_face_normal_point.m_x;
					projected_triangle.m_face_normal.m_y = projected_face_normal_point.m_y;
				}
			}
			// the wrong way to do this!
			// to draw face normal we scale the mesh up
			// find the center of the scaled triangle
			// and then draw a line between the non scaled center
			// and the scaled center
			//std::vector<vector::Vector4d> transformed_vertices_scaled{ };
			//for (const auto& vertex : face_vertices)
			//{
			//	vector::Vector4d point{ vertex };
			//	matrix::Matrix4x4 world_matrix{};
			//	// order matters scale, then rotate and then translate
			//	world_matrix *= scale_face_center_matrix;
			//	world_matrix *= rotatation_matrix_z;
			//	world_matrix *= rotatation_matrix_y;
			//	world_matrix *= rotatation_matrix_x;
			//	world_matrix *= translation_matrix;
			//	point = world_matrix.mult_vec4d(point);
			//	transformed_vertices_scaled.push_back(point);
			//}
			//vector::Vector3d face_center_scaled{ mesh_to_render.get_face_center(transformed_vertices_scaled) };
			//vector::Vector4d projected_center_scaled{ projection_matrix.project(face_center_scaled) };
			//vector::Vector2d<double> projected_center_scaled_point{ projected_center_scaled.m_x, projected_center_scaled.m_y };
			//projected_center_scaled_point.m_x *= display_mode->w / 2;
			//projected_center_scaled_point.m_y *= display_mode->h / 2;
			//projected_center_scaled_point.m_x += display_mode->w / 2;
			//projected_center_scaled_point.m_y += display_mode->h / 2;
			/*projected_triangle.m_face_normal.m_x = projected_center_scaled_point.m_x;
			projected_triangle.m_face_normal.m_y = projected_center_scaled_point.m_y;*/

			triangles_to_render.push_back(projected_triangle);
		}
		counter++;
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
	bool& render_face_center,
	bool& render_normals,
	const SDLWrapper& sdl
)
{
	bool render_wireframe{ false };
	bool render_vertex{ false };
	bool render_shaded{ false };
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
				2,
				2,
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
	const vector::Vector3d camera_postion{ 0.0, 0.0, 0.0 };
	int previous_frame_time{ 0 };
	geo::Mesh mesh{ ".\\assets\\sphere.obj" };
	/*geo::Mesh mesh2{ ".\\assets\\sphere.obj" };
	std::vector<geo::Mesh> meshes{ mesh, mesh2 };*/
	std::vector<geo::Mesh> meshes{ mesh };
	int render_mode{ display::RenderModes::wireframe };
	bool render_face_center{ false };
	bool render_normals{ false };
	bool backface_culling{ true };
	constexpr const double fov{ M_PI / 3.0 }; // fov in radians (60 deg)
	const double aspect{ static_cast<double>(display_mode.h) / static_cast<double>(display_mode.w) };
	const double znear{ 0.1 };
	const double zfar{ 100.0 };
	matrix::Matrix4x4 projection_matrix{fov, aspect, znear, zfar};
	while (is_running)
	{
		input.process(is_running, render_mode, backface_culling, event, sdl);
		update(
			meshes,
			projection_matrix,
			triangles_to_render,
			camera_postion,
			display,
			&display_mode,
			previous_frame_time,
			backface_culling,
			directional_light,
			render_face_center,
			render_normals,
			sdl);
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
			render_face_center,
			render_normals,
			sdl
		);
	}
	display.cleanup(window, renderer, colour_buffer, sdl);
	return 0;
}
