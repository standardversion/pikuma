#pragma once
#include <vector>
#include "SDL.h"
#include "vector2d.h"
#include "vector3d.h"
#include "vector4d.h"
#include "triangle.h"

namespace geo
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::vector<vector::Vector3d>& vertices, const std::vector<geo::face_t>& faces);
		Mesh(const char* obj_filename, const char* texture_filename, const vector::Vector3d& rotation, const vector::Vector3d& scale, const vector::Vector3d& translate);
		vector::Vector3d get_face_center(const std::vector<vector::Vector4d>& face_vertices) const;
		vector::Vector3d get_face_normal(const std::vector<vector::Vector4d>& face_vertices) const;
		std::vector<vector::Vector3d> get_per_vertex_normals(const std::vector<vector::Vector4d>& face_vertices) const;

		std::vector<vector::Vector3d> m_vertices{};
		std::vector<vector::Vector3d> m_vertex_normals{};
		std::vector<vector::Vector2d<double>> m_uvs{};
		std::vector<geo::face_t> m_faces{};
		vector::Vector3d m_rotation{ 0.0, 0.0, 0.0 };
		vector::Vector3d m_scale{ 1.0, 1.0, 1.0 };
		vector::Vector3d m_translation{ 0.0, 0.0, 0.0 };
		SDL_Surface* m_surface{ nullptr };
	};
}