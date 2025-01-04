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
		Mesh(std::vector<vector::Vector3d> vertices, std::vector<tri::face_t> faces);
		const std::vector<vector::Vector3d>& get_vertices() const;
		const std::vector<tri::face_t>& get_faces() const;
		const vector::Vector3d& get_rotation() const;
		vector::Vector3d& get_rotation();
		void push_vertex(vector::Vector3d vertex);
		void push_face(tri::face_t face);
	private:
		std::vector<vector::Vector3d> m_vertices{};
		std::vector<tri::face_t> m_faces{};
		vector::Vector3d m_rotation{ 0.0, 0.0, 0.0 };
	};
}