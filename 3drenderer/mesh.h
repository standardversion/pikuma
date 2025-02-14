#pragma once
#include <vector>
#include "vector3d.h"
#include "triangle.h"

namespace geo
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::vector<vector::Vector3d>& vertices, const std::vector<geo::face_t>& faces);
		Mesh(const char* filename);
		vector::Vector3d get_face_center(const std::vector<vector::Vector3d>& face_vertices) const;
		vector::Vector3d get_face_normal(const std::vector<vector::Vector3d>& face_vertices) const;
		vector::Vector3d get_face_normal(const vector::Vector3d& face_center, const std::vector<vector::Vector3d>& face_vertices) const;

		std::vector<vector::Vector3d> m_vertices{};
		std::vector<geo::face_t> m_faces{};
		vector::Vector3d m_rotation{ 0.0, 0.0, 0.0 };
	};
}