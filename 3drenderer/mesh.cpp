#include "mesh.h"

namespace mesh
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(std::vector<vector::Vector3d> vertices, std::vector<tri::face_t> faces)
		: m_vertices{ vertices }, m_faces{ faces }
	{
	}

	const std::vector<vector::Vector3d>& Mesh::get_vertices() const
	{
		return m_vertices;
	}

	const std::vector<tri::face_t>& Mesh::get_faces() const
	{
		return m_faces;
	}

	const vector::Vector3d& Mesh::get_rotation() const
	{
		return m_rotation;
	}

	vector::Vector3d& Mesh::get_rotation()
	{
		return m_rotation;
	}

	void Mesh::push_vertex(vector::Vector3d vertex)
	{
		m_vertices.push_back(vertex);
	}

	void Mesh::push_face(tri::face_t face)
	{
		m_faces.push_back(face);
	}
}
