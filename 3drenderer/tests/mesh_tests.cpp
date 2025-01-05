#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../mesh.h"

namespace mesh_tests
{

	TEST(mesh, file_constructor)
	{
		geo::Mesh mesh{ "..\\..\\tests\\assets\\mock.obj" };
		EXPECT_EQ(mesh.m_vertices.size(), 3);
		EXPECT_EQ(mesh.m_vertices[0].m_x, 1.0);
		EXPECT_EQ(mesh.m_vertices[0].m_y, 1.0);
		EXPECT_EQ(mesh.m_vertices[0].m_z, -1.0);
		EXPECT_EQ(mesh.m_vertices[1].m_x, 1.0);
		EXPECT_EQ(mesh.m_vertices[1].m_y, -1.0);
		EXPECT_EQ(mesh.m_vertices[1].m_z, -1.0);
		EXPECT_EQ(mesh.m_vertices[2].m_x, 1.0);
		EXPECT_EQ(mesh.m_vertices[2].m_y, 1.0);
		EXPECT_EQ(mesh.m_vertices[2].m_z, 1.0);
		EXPECT_EQ(mesh.m_faces.size(), 1);
		EXPECT_EQ(mesh.m_faces[0].a, 2);
		EXPECT_EQ(mesh.m_faces[0].b, 3);
		EXPECT_EQ(mesh.m_faces[0].c, 1);
	}

}