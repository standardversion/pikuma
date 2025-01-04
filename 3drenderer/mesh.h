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

		std::vector<vector::Vector3d> m_vertices{};
		std::vector<geo::face_t> m_faces{};
		vector::Vector3d m_rotation{ 0.0, 0.0, 0.0 };
	};
}