#pragma once
#include <vector>>
#include "vector2d.h"
#include "vector3d.h"
#include "vector4d.h"
#include "clipping.h"


namespace geo
{
	class Polygon
	{
	public:
		Polygon(const std::vector<vector::Vector4d>& vertices, const std::vector<vector::Vector3d>& normals, const std::vector<vector::Vector2d<double>>& uvs);
		void clip(const std::vector<clip::plane_t>& clipping_planes);
		void split_into_tris(
			std::vector<std::vector<vector::Vector4d>>& vertices,
			std::vector<std::vector<vector::Vector2d<double>>>& uvs,
			std::vector<std::vector<vector::Vector3d>>& normals
		) const;

		std::vector<vector::Vector3d> m_vertices;
		std::vector<vector::Vector3d> m_normals;
		std::vector<vector::Vector2d<double>> m_uvs;
	};
}
