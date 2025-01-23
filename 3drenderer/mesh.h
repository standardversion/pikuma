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

		std::vector<vector::Vector3d> m_vertices{};
		std::vector<geo::face_t> m_faces{};
		vector::Vector3d m_rotation{ 0.0, 0.0, 0.0 };
		vector::Vector3d m_scale{ 1.0, 1.0, 1.0 };
		vector::Vector3d m_translation{ 0.0, 0.0, 0.0 };
	};
}