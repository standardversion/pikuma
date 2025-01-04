#pragma once
#include <vector>
#include "vector3d.h"
#include "triangle.h"

namespace mesh
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::vector<vector::Vector3d>& vertices, const std::vector<tri::face_t>& faces);

		std::vector<vector::Vector3d> m_vertices{};
		std::vector<tri::face_t> m_faces{};
		vector::Vector3d m_rotation{ 0.0, 0.0, 0.0 };
	};
}