#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../triangle.h"

namespace triangle_tests
{

	TEST(triangle, sort_1)
	{
		std::vector<vector::Vector2d<double>> points{
			{ 10.0, 5.0 },
			{1.0, 2.0},
			{ 10.0, 3.0 }
		};
		geo::Triangle<double> triangle{
			points
		};
		triangle.sort_vertices_by_y();
		EXPECT_EQ(triangle.m_points[0].m_y, 2.0);
		EXPECT_EQ(triangle.m_points[0].m_x, 1.0);
		EXPECT_EQ(triangle.m_points[1].m_y, 3.0);
		EXPECT_EQ(triangle.m_points[1].m_x, 10.0);
		EXPECT_EQ(triangle.m_points[2].m_y, 5.0);
		EXPECT_EQ(triangle.m_points[2].m_x, 10.0);
	}

	TEST(triangle, sort_2)
	{
		std::vector<vector::Vector2d<double>> points{
			{1.0, 2.0},
			{ 10.0, 5.0 },
			{ 10.0, 3.0 }
		};
		geo::Triangle<double> triangle{
			points
		};
		triangle.sort_vertices_by_y();
		EXPECT_EQ(triangle.m_points[0].m_y, 2.0);
		EXPECT_EQ(triangle.m_points[0].m_x, 1.0);
		EXPECT_EQ(triangle.m_points[1].m_y, 3.0);
		EXPECT_EQ(triangle.m_points[1].m_x, 10.0);
		EXPECT_EQ(triangle.m_points[2].m_y, 5.0);
		EXPECT_EQ(triangle.m_points[2].m_x, 10.0);
	}

	TEST(triangle, sort_3)
	{
		std::vector<vector::Vector2d<double>> points{
			{1.0, 2.0},
			{ 10.0, 3.0 },
			{ 10.0, 5.0 }
		};
		geo::Triangle<double> triangle{
			points
		};
		triangle.sort_vertices_by_y();
		EXPECT_EQ(triangle.m_points[0].m_y, 2.0);
		EXPECT_EQ(triangle.m_points[0].m_x, 1.0);
		EXPECT_EQ(triangle.m_points[1].m_y, 3.0);
		EXPECT_EQ(triangle.m_points[1].m_x, 10.0);
		EXPECT_EQ(triangle.m_points[2].m_y, 5.0);
		EXPECT_EQ(triangle.m_points[2].m_x, 10.0);
	}

	TEST(triangle, get_inverse_slope)
	{
		std::vector<vector::Vector2d<int>> points{
			{ 10, 5 },
			{ 20, 5 },
			{ 15, 10 },
		};
		geo::Triangle<int> triangle{
			points
		};
		double slope{ triangle.get_inverse_slope(2, 0) };
		EXPECT_EQ(slope, 1.0);
		double slope2{ triangle.get_inverse_slope(2, 1) };
		EXPECT_EQ(slope2, -1.0);
	}

	TEST(triangle, get_midpoint)
	{
		std::vector<vector::Vector2d<int>> points{
			{ 0, 0 },
			{ 20, 5 },
			{ 15, 10 },
		};
		geo::Triangle<int> triangle{
			points
		};
		vector::Vector2d<int> md{ triangle.get_midpoint() };
		EXPECT_EQ(md.m_x, 7);
		EXPECT_EQ(md.m_y, 5);
	}
}