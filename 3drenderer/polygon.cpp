#include "polygon.h"
#include "utils.h"

namespace geo
{
	Polygon::Polygon(const std::vector<vector::Vector4d>& vertices, const std::vector<vector::Vector3d>& normals, const std::vector<vector::Vector2d<double>>& uvs)
		:m_normals{ normals }, m_uvs{ uvs } 
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
			std::vector<vector::Vector2d<double>> inside_uvs{};
			std::vector<vector::Vector3d> inside_normals{};

			// start the current vertex at the first vertex of the polyg, and the previous vertes with the last vertex of the poly
			auto current_vertex{ m_vertices.begin()};
			auto current_uv{ m_uvs.begin() };
			auto current_normal{ m_normals.begin() };
			

			for (std::size_t i = 0; i < m_vertices.size(); i++)
			{
				auto previous_vertex = (current_vertex == m_vertices.begin()) ? m_vertices.end() - 1 : std::prev(current_vertex);
				auto previous_uv = (current_uv == m_uvs.begin()) ? m_uvs.end() - 1 : std::prev(current_uv);
				auto previous_normal = (current_normal == m_normals.begin()) ? m_normals.end() - 1 : std::prev(current_normal);
				double previous_dot{ plane.m_normal.dot_product(*previous_vertex - plane.m_point) };
				double current_dot{ plane.m_normal.dot_product(*current_vertex - plane.m_point) };
				// if we changed from inside to outside or from outside to inside
				if (current_dot * previous_dot < 0)
				{
					// find interpolation factor t
					double t{ previous_dot / (previous_dot - current_dot) };
					vector::Vector3d intersection_point{
						utils::lerp(previous_vertex->m_x, current_vertex->m_x, t),
						utils::lerp(previous_vertex->m_y, current_vertex->m_y, t),
						utils::lerp(previous_vertex->m_z, current_vertex->m_z, t)
					};
					inside_vertices.emplace_back(intersection_point);
					vector::Vector2d<double> intersection_uv{
						utils::lerp(previous_uv->m_x, current_uv->m_x, t),
						utils::lerp(previous_uv->m_y, current_uv->m_y, t)
					};
					inside_uvs.emplace_back(intersection_uv);
					vector::Vector3d intersection_normal{
						utils::lerp(previous_normal->m_x, current_normal->m_x, t),
						utils::lerp(previous_normal->m_y, current_normal->m_y, t),
						utils::lerp(previous_normal->m_z, current_normal->m_z, t)
					};
					intersection_normal.normalize();
					inside_normals.emplace_back(intersection_normal);
				}

				// vertex is inside the plane
				if (current_dot > 0)
				{
					inside_vertices.emplace_back(*current_vertex);
					inside_uvs.emplace_back(*current_uv);
					inside_normals.emplace_back(*current_normal);
				}
				// move the pointer to the next vertex
				previous_vertex = current_vertex;
				previous_uv = current_uv;
				previous_normal = current_normal;
				previous_dot = current_dot;
				current_vertex++;
				current_uv++;
				current_normal++;
			}
			// copy the list of inside vertices into the destination polygon;
			m_vertices = inside_vertices;
			m_uvs = inside_uvs;
			m_normals = inside_normals;
		}
	}

	void Polygon::split_into_tris(
		std::vector<std::vector<vector::Vector4d>>& vertices_array,
		std::vector<std::vector<vector::Vector2d<double>>>& uvs_array,
		std::vector<std::vector<vector::Vector4d>>& normals_array
	) const
	{

		std::vector<std::vector<vector::Vector4d>> triangles{};
		if (m_vertices.size())
		{
			for (std::size_t i{ 0 }; i < m_vertices.size() - 2; i++)
			{
				std::vector<vector::Vector4d> vertex_array(3);
				std::vector<vector::Vector2d<double>> uv_array{};
				std::vector<vector::Vector4d> normal_array(3);
				std::size_t index0{ 0 };
				std::size_t index1{ i + 1 };
				std::size_t index2{ i + 2 };
				vertex_array[0] = m_vertices[index0];
				vertex_array[1] = m_vertices[index1];
				vertex_array[2] = m_vertices[index2];
				vertices_array.emplace_back(vertex_array);
				uv_array.emplace_back(m_uvs[index0]);
				uv_array.emplace_back(m_uvs[index1]);
				uv_array.emplace_back(m_uvs[index2]);
				uvs_array.emplace_back(uv_array);
				normal_array[0] = m_normals[index0];
				normal_array[1] = m_normals[index1];
				normal_array[2] = m_normals[index2];
				normals_array.emplace_back(normal_array);
			}
		}
		return;
	}
}