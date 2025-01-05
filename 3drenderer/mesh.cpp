#include <fstream>
#include <string>
#include <string_view>
#include <vector>
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

		std::ifstream file(obj_filepath);
		if (!file)
		{
			//handle file does not exist
		}
		std::string line{};
		while (std::getline(file, line))
		{
			std::vector<std::string> parts{ utils::split(line, " ")};
			if (parts[0] == "v")
			{
				// vertex line in obj: v 1.000000 1.000000 -1.000000
				m_vertices.emplace_back(std::stod(parts[1]), std::stod(parts[2]), std::stod(parts[3]));
			}
			else if (parts[0] == "f")
			{
				// face line in obj: f 5/1/1 3/2/1 1/3/1, 5, 3, 1 are the vertex indices
				std::vector<std::string> face_parts1{ utils::split(parts[1], "/")};
				std::vector<std::string> face_parts2{ utils::split(parts[2], "/")};
				std::vector<std::string> face_parts3{ utils::split(parts[3], "/")};
				m_faces.emplace_back(std::stoi(face_parts1[0]), std::stoi(face_parts2[0]), std::stoi(face_parts3[0]));
			}
		}
	}

}
