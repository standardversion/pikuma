#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include "mesh.h"
#include "utils.h"

namespace geo
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const std::vector<vector::Vector3d>& vertices, const std::vector<geo::face_t>& faces)
		: m_vertices{ vertices }, m_faces{ faces }
	{
	}

	Mesh::Mesh(const char* obj_filepath)
	{
		const std::filesystem::path filepath{ std::filesystem::absolute(obj_filepath) };
		if (!std::filesystem::exists(filepath))
		{
			//handle file does not exist
		}
		else {
			std::ifstream file(filepath);

			std::string line{};
			while (std::getline(file, line))
			{
				std::vector<std::string> parts{ utils::split(line, " ") };
				if (parts.size()) {
					if (parts[0] == "v")
					{
						// vertex line in obj: v 1.000000 1.000000 -1.000000
						m_vertices.emplace_back(std::stod(parts[1]), std::stod(parts[2]), std::stod(parts[3]));
					}
					else if (parts[0] == "f")
					{
						// face line in obj: f 5/1/1 3/2/1 1/3/1, 5, 3, 1 are the vertex indices
						std::vector<std::string> face_parts1{ utils::split(parts[1], "/") };
						std::vector<std::string> face_parts2{ utils::split(parts[2], "/") };
						std::vector<std::string> face_parts3{ utils::split(parts[3], "/") };
						m_faces.emplace_back(std::stoi(face_parts1[0]), std::stoi(face_parts2[0]), std::stoi(face_parts3[0]));
					}
				}
			}
		}
	}

	vector::Vector3d Mesh::get_face_center(const std::vector<vector::Vector3d>& face_vertices) const
	{
		return vector::Vector3d{
			(face_vertices[0].m_x + face_vertices[1].m_x + face_vertices[2].m_x) / 3.0,
			(face_vertices[0].m_y + face_vertices[1].m_y + face_vertices[2].m_y) / 3.0,
			(face_vertices[0].m_z + face_vertices[1].m_z + face_vertices[2].m_z) / 3.0,
		};
	}

	vector::Vector3d Mesh::get_face_normal(const std::vector<vector::Vector3d>& face_vertices) const
	{
		const vector::Vector3d face_center{ get_face_center(face_vertices) };
		vector::Vector3d vector_am{ face_vertices[0] - face_center };
		vector::Vector3d vector_bm{ face_vertices[1] - face_center };
		vector_am.normalize();
		vector_bm.normalize();
		return vector_bm.cross_product(vector_am);
	}
}
