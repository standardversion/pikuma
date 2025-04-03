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
#include "vector3d.h"
#include "vector2d.h"
#include "mesh.h"
#include "triangle.h"
#include "utils.h"
#include "matrix.h"
#include "camera.h"
#include "clipping.h"
#include "polygon.h"

void update(
	std::vector<geo::Mesh>& meshes,
	const matrix::Matrix4x4& projection_matrix,
	std::vector<geo::Triangle<int>>& triangles_to_render,
	camera::camera_t& view_camera,
	std::vector<clip::plane_t>& clipping_planes,
	int& previous_frame_time,
	double& delta_time,
	const bool backface_culling,
	const shading::Light& light,
	const bool render_face_center,
	const bool render_normals
)
{
	int time_to_wait = display::FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= display::FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}

	// get a delta time factor converted to seconds to be used to update our objs
	delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

	previous_frame_time = SDL_GetTicks();

	triangles_to_render = {};
	vector::Vector3d origin{ 0.0, 0.0, 0.0 };
	vector::Vector3d up{ 0.0, 1.0, 0.0 };
	for (auto& mesh_to_render : meshes)
	{
		mesh_to_render.m_rotation.m_x += 0.2 * delta_time;
		mesh_to_render.m_rotation.m_y += 0.2 * delta_time;
		mesh_to_render.m_rotation.m_z += 0.2 * delta_time;
		//mesh_to_render.m_scale.m_x = 0.5;
		//mesh_to_render.m_scale.m_y = 0.5;
		//mesh_to_render.m_scale.m_z = 0.5;
		//mesh_to_render.m_translation.m_x += 0.01;
		// move pionts away from camera
		mesh_to_render.m_translation.m_z = 5.0;

		vector::Vector3d target{ 0.0, 0.0, 1.0 };
		matrix::Matrix4x4 camera_yaw_rotation{ matrix::Matrix4x4::make_rotation_matrix(view_camera.m_yaw, 'y')};
		matrix::Matrix4x4 camera_pitch_rotation{ matrix::Matrix4x4::make_rotation_matrix(view_camera.m_pitch, 'x') };
		matrix::Matrix4x4 camera_rotation{};
		camera_rotation *= camera_pitch_rotation;
		camera_rotation *= camera_yaw_rotation;
		view_camera.m_direction = camera_rotation.mult_vec4d(target);

		//offset the camera position in the direction where the camera is pointing at
		target = view_camera.m_position + view_camera.m_direction;
		// create the view matrix looking at a hardcoded targetpoint
		matrix::Matrix4x4 view_matrix{ matrix::Matrix4x4::make_view_matrix(view_camera.m_position, target, up) };

		matrix::Matrix4x4 scale_matrix{ matrix::Matrix4x4::make_scale_matrix(mesh_to_render.m_scale.m_x, mesh_to_render.m_scale.m_y, mesh_to_render.m_scale.m_z) };
		matrix::Matrix4x4 translation_matrix{ matrix::Matrix4x4::make_translation_matrix(mesh_to_render.m_translation.m_x, mesh_to_render.m_translation.m_y, mesh_to_render.m_translation.m_z) };
		matrix::Matrix4x4 rotatation_matrix_x{ matrix::Matrix4x4::make_rotation_matrix(mesh_to_render.m_rotation.m_x, 'x') };
		matrix::Matrix4x4 rotatation_matrix_y{ matrix::Matrix4x4::make_rotation_matrix(mesh_to_render.m_rotation.m_y, 'y') };
		matrix::Matrix4x4 rotatation_matrix_z{ matrix::Matrix4x4::make_rotation_matrix(mesh_to_render.m_rotation.m_z, 'z') };

		for (const auto& face : mesh_to_render.m_faces)
		{
			//each mesh face has 3 vertices stored as the index of the vertex in mesh_vertices
			//face_vertices is an array of 3 vector3d  containing x,y,z coordinates of each vertex
			std::vector<vector::Vector3d> face_vertices{
				mesh_to_render.m_vertices[face.a - 1],
				mesh_to_render.m_vertices[face.b - 1],
				mesh_to_render.m_vertices[face.c - 1]
			};
			// normals
			std::vector<vector::Vector3d> face_vtx_normals{
				mesh_to_render.m_vertex_normals[face.a - 1],
				mesh_to_render.m_vertex_normals[face.b - 1],
				mesh_to_render.m_vertex_normals[face.c - 1]

			};

			std::vector<vector::Vector4d> transformed_vertices{};
			std::vector<vector::Vector3d> transformed_normals{};
			// apply transformations
			std::size_t counter{ 0 };
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
				point = view_matrix.mult_vec4d(point);
				transformed_vertices.push_back(point);
				
				vector::Vector4d transformed_normal{ face_vtx_normals[counter] };
				transformed_normal = world_matrix.get_inverse().get_transpose().mult_vec4d(transformed_normal);
				transformed_normal = view_matrix.get_inverse().get_transpose().mult_vec4d(transformed_normal);
				vector::Vector3d transformed_normal_vec3{ transformed_normal };
				transformed_normal_vec3.normalize();
				transformed_normals.emplace_back(transformed_normal_vec3);
				counter++;
			}

			std::vector<vector::Vector3d> per_vertex_normals{ mesh_to_render.get_per_vertex_normals(transformed_vertices) };
			if (backface_culling) {
				// find the camera ray ie the vector between a point in the triangle and the camera origin
				vector::Vector3d camera_ray{ origin - transformed_vertices[0] };

				// calculate how aligned the camera ray is with the face normal (using dot product)
				if (camera_ray.dot_product(per_vertex_normals[0]) < 0)
				{
					continue;
				}
			}

			//clipping
			//loop through each vertex and project the points of those faces
			// convert the triangle points to ints as we'll project to screen space so we're dealing with x & y pixels which are in ints

			// uvs don't need to be transformed
			std::vector<vector::Vector2d<double>> face_vtx_uvs{
				mesh_to_render.m_uvs[face.a_uv - 1],
				mesh_to_render.m_uvs[face.b_uv - 1],
				mesh_to_render.m_uvs[face.c_uv - 1]
			};
			
			geo::Polygon poly{ transformed_vertices, transformed_normals, face_vtx_uvs };
			poly.clip(clipping_planes);
			std::vector<std::vector<vector::Vector4d>> clipped_vertices_array{};
			std::vector<std::vector<vector::Vector2d<double>>> clipped_uvs_array{};
			std::vector<std::vector<vector::Vector3d>> clipped_normals_array{};
			poly.split_into_tris(clipped_vertices_array, clipped_uvs_array, clipped_normals_array);
			
			std::size_t clipped_entities_counter{ 0 };
			for (const auto& clipped_vertex_array : clipped_vertices_array)
			{
				geo::Triangle<int> triangle_to_render{ };
				double light_intensity{ -per_vertex_normals[0].dot_product(light.m_direction) };
				triangle_to_render.m_light_intensity = light_intensity;
				triangle_to_render.m_surface = mesh_to_render.m_surface;
				// project the triangle
				counter = 0;
				for (const auto& vertex : clipped_vertex_array)
				{
					vector::Vector4d projected_point{ display::project_vec4d(projection_matrix, vertex) };
					triangle_to_render.m_per_vtx_lt_intensity.push_back(-clipped_normals_array[clipped_entities_counter][counter].dot_product(light.m_direction));
					triangle_to_render.m_points.push_back(vector::Vector2d<int>{static_cast<int>(projected_point.m_x), static_cast<int>(projected_point.m_y)});
					triangle_to_render.m_points_z.push_back(projected_point.m_z);
					triangle_to_render.m_points_w.push_back(projected_point.m_w);
					triangle_to_render.m_uvs.push_back(clipped_uvs_array[clipped_entities_counter][counter]);
					counter++;
				}
				if (render_face_center || render_normals)
				{
					vector::Vector3d face_center{ mesh_to_render.get_face_center(clipped_vertex_array) };
					vector::Vector4d projected_center_point{ display::project_vec4d(projection_matrix, face_center) };

					triangle_to_render.m_center.m_x = projected_center_point.m_x;
					triangle_to_render.m_center.m_y = projected_center_point.m_y;
					if (render_normals)
					{
						vector::Vector3d face_normal{ clipped_normals_array[clipped_entities_counter][0] };
						// move the normal to the center of the face
						face_normal.m_x += face_center.m_x;
						face_normal.m_y += face_center.m_y;
						face_normal.m_z += face_center.m_z;
						face_normal.normalize();

						vector::Vector4d projected_face_normal_point{ display::project_vec4d(projection_matrix, face_normal) };

						triangle_to_render.m_face_normal.m_x = projected_face_normal_point.m_x;
						triangle_to_render.m_face_normal.m_y = projected_face_normal_point.m_y;
					}
				}

				triangles_to_render.push_back(triangle_to_render);
				clipped_entities_counter++;
			}
		}
	}
}

void render(
	const std::uint32_t edge_colour,
	const std::uint32_t bg_colour,
	const std::uint32_t vertex_colour,
	const std::uint32_t fill_colour,
	const int grid_on,
	std::vector<geo::Triangle<int>>& triangles_to_render,
	const int render_mode,
	bool& render_face_center,
	bool& render_normals,
	bool render_flat_shaded,
	bool render_gouraud_shaded,
	bool render_texture
)
{
	bool render_wireframe{ false };
	bool render_vertex{ false };
	bool render_shaded{ false };
	display::activate_render_mode(render_mode, render_wireframe, render_vertex, render_shaded, render_face_center, render_normals);
	//display::draw_grid(edge_colour, bg_colour, grid_on);

	for (auto& triangle : triangles_to_render)
	{
		if (render_shaded)
		{
			triangle.fill(
				render_flat_shaded,
				render_gouraud_shaded,
				render_texture,
				fill_colour
			);
		}
		
		if (render_wireframe)
		{
			triangle.draw(
				edge_colour
			);
		}
		
		if (render_vertex)
		{
			for (const auto& point : triangle.m_points)
			{
				display::draw_rect(
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
			display::draw_rect(
				triangle.m_center.m_x,
				triangle.m_center.m_y,
				2,
				2,
				vertex_colour
			);
		}
		if (render_normals)
		{
			display::draw_line(
				triangle.m_center.m_x,
				triangle.m_center.m_y,
				triangle.m_face_normal.m_x,
				triangle.m_face_normal.m_y,
				edge_colour
			);
		}
	}

	// render the colour buffer
	display::render_colour_buffer();
	// fill the colour buffer with a colour value
	display::clear_colour_buffer(bg_colour);

	display::clear_z_buffer();

	// Update the screen with any rendering performed since the previous call.
	display::render_present();
}


int main(int argc, char* argv[])
{
	bool is_running{ display::setup() };
	vector::Vector2d screen_dimensions{ display::get_display_width_height() };
	constexpr const std::uint32_t bg_colour{ 0x00000000 };
	constexpr const std::uint32_t edge_colour{ 0xFFFFFFFF };
	constexpr const std::uint32_t vertex_colour{ 0xFFFFFF00 };
	constexpr const std::uint32_t fill_colour{ 0x00808080 };
	const shading::Light directional_light{};
	std::vector<geo::Triangle<int>> triangles_to_render{};
	camera::camera_t view_camera{ .m_position{ 0.0, 0.0, 0.0 }, .m_direction{ 0.0, 0.0, 1.0 }, .m_forward_velocity{ 0.0, 0.0, 0.0 }, .m_yaw{ 0.0 }, .m_pitch{ 0.0 } };
	int previous_frame_time{ 0 };
	double delta_time{ 0.0 };
	geo::Mesh mesh{ ".\\assets\\f22.obj", ".\\assets\\f22.png", {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {-3.0, 0.0, 0.0} };
	geo::Mesh mesh2{ ".\\assets\\f117.obj", ".\\assets\\f117.png", {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0} };
	geo::Mesh mesh3{ ".\\assets\\efa.obj", ".\\assets\\efa.png", {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {3.0, 0.0, 0.0} };
	std::vector<geo::Mesh> meshes{ mesh, mesh2, mesh3 };
	int render_mode{ display::RenderModes::wireframe };
	bool render_face_center{ false };
	bool render_normals{ false };
	bool backface_culling{ true };
	bool render_flat_shaded{ false };
	bool render_gouraud_shaded{ false };
	bool render_texture{ false };
	const double aspect_x{ static_cast<double>(screen_dimensions.m_x) / static_cast<double>(screen_dimensions.m_y) };
	const double aspect_y{ static_cast<double>(screen_dimensions.m_y) / static_cast<double>(screen_dimensions.m_x) };
	constexpr const double fov_y{ M_PI / 3.0 }; // fov in radians (60 deg)
	const double fov_x{ atan(tan(fov_y / 2) * aspect_x) * 2.0 }; // fov in radians (60 deg)
	const double z_near{ 0.1 };
	const double z_far{ 100.0 };
	matrix::Matrix4x4 projection_matrix{fov_y, aspect_y, z_near, z_far};
	std::vector<clip::plane_t> clipping_planes{ clip::init_frustum_planes(fov_x, fov_y, z_near, z_far) };
	while (is_running)
	{
		input::process(is_running, view_camera, delta_time, render_mode, backface_culling, render_flat_shaded, render_gouraud_shaded, render_texture);
		update(
			meshes,
			projection_matrix,
			triangles_to_render,
			view_camera,
			clipping_planes,
			previous_frame_time,
			delta_time,
			backface_culling,
			directional_light,
			render_face_center,
			render_normals);
		render(
			edge_colour,
			bg_colour,
			vertex_colour,
			fill_colour,
			10,
			triangles_to_render,
			render_mode,
			render_face_center,
			render_normals,
			render_flat_shaded,
			render_gouraud_shaded,
			render_texture
		);
	}
	display::cleanup();
	return 0;
}
