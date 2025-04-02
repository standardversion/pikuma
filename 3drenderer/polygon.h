#pragma once
#include <vector>>
#include "vector4d.h"
#include "vector3d.h"
#include "clipping.h"


namespace geo
{
	class Polygon
	{
	public:
		Polygon(const std::vector<vector::Vector4d>& vertices, const std::vector<vector::Vector3d>& normals);
		void clip(const std::vector<clip::plane_t>& clipping_planes);
		std::vector<std::vector<vector::Vector4d>> split_into_tris() const;

		std::vector<vector::Vector3d> m_vertices;
		std::vector<vector::Vector3d> m_normals;
	};
}
