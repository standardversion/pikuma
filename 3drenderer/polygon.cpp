#include "polygon.h"

namespace geo
{
	Polygon::Polygon(const std::vector<vector::Vector4d>& vertices, const std::vector<vector::Vector3d>& normals)
		:m_normals{ normals }
	{
		for (const auto& vertex : vertices)
		{
			m_vertices.emplace_back(vertex);
		}
	}

	void Polygon::clip(const std::vector<clip::plane_t>& clipping_planes)
	{
		
		for (const auto& plane : clipping_planes)
		{
			// array of vertices which will make up the final polygon
			std::vector<vector::Vector3d> inside_vertices{};

			// start the current vertex at the first vertex of the polyg, and the previous vertes with the last vertex of the poly
			auto current_vertex{ m_vertices.begin()};
			

			for (std::size_t i = 0; i < m_vertices.size(); i++)
			{
				auto previous_vertex = (current_vertex == m_vertices.begin()) ? m_vertices.end() - 1 : std::prev(current_vertex);
				double previous_dot{ plane.m_normal.dot_product(*previous_vertex - plane.m_point) };
				double current_dot{ plane.m_normal.dot_product(*current_vertex - plane.m_point) };
				// if we changed from inside to outside or from outside to inside
				if (current_dot * previous_dot < 0)
				{
					// find interpolation factor t
					double t{ previous_dot / (previous_dot - current_dot) };
					// calculate the intersection point I = Q2 + t(Q1 - Q2);
					vector::Vector3d intersection_point{ *previous_vertex + (*current_vertex - *previous_vertex) * t };
					inside_vertices.emplace_back(intersection_point);
				}

				// vertex is inside the plane
				if (current_dot > 0)
				{
					inside_vertices.emplace_back(*current_vertex);
				}
				// move the pointer to the next vertex
				previous_vertex = current_vertex;
				previous_dot = current_dot;
				current_vertex++;
			}
			// copy the list of inside vertices into the destination polygon;
			m_vertices = inside_vertices;
		}
	}

	std::vector<std::vector<vector::Vector4d>> Polygon::split_into_tris() const
	{

		std::vector<std::vector<vector::Vector4d>> triangles{};
		if (m_vertices.size())
		{
			for (std::size_t i{ 0 }; i < m_vertices.size() - 2; i++)
			{
				std::vector<vector::Vector4d> tri_verts(3);
				std::size_t index0{ 0 };
				std::size_t index1{ i + 1 };
				std::size_t index2{ i + 2 };
				tri_verts[0] = m_vertices[index0];
				tri_verts[1] = m_vertices[index1];
				tri_verts[2] = m_vertices[index2];
				triangles.emplace_back(tri_verts);
			}
		}
		return triangles;
	}
}