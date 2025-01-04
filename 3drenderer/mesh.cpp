#include "mesh.h"

namespace mesh
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const std::vector<vector::Vector3d>& vertices, const std::vector<tri::face_t>& faces)
		: m_vertices{ vertices }, m_faces{ faces }
	{
	}
}
