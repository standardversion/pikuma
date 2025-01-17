#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../triangle.h"

namespace triangle_tests
{

	TEST(triangle, sort_1)
	{
		std::vector<vector::Vector2d> points{
			{ 10.0, 5.0 },
			{1.0, 2.0},
			{ 10.0, 3.0 },
		};
		geo::Triangle triangle{
			points
		};
		triangle.sort();
		EXPECT_EQ(triangle.m_points[0].m_y, 2.0);
		EXPECT_EQ(triangle.m_points[0].m_x, 1.0);
		EXPECT_EQ(triangle.m_points[1].m_y, 3.0);
		EXPECT_EQ(triangle.m_points[1].m_x, 10.0);
		EXPECT_EQ(triangle.m_points[2].m_y, 5.0);
		EXPECT_EQ(triangle.m_points[2].m_x, 10.0);
	}

	TEST(triangle, sort_2)
	{
		std::vector<vector::Vector2d> points{
			{1.0, 2.0},
			{ 10.0, 5.0 },
			{ 10.0, 3.0 },
			
		};
		geo::Triangle triangle{
			points
		};
		triangle.sort();
		EXPECT_EQ(triangle.m_points[0].m_y, 2.0);
		EXPECT_EQ(triangle.m_points[0].m_x, 1.0);
		EXPECT_EQ(triangle.m_points[1].m_y, 3.0);
		EXPECT_EQ(triangle.m_points[1].m_x, 10.0);
		EXPECT_EQ(triangle.m_points[2].m_y, 5.0);
		EXPECT_EQ(triangle.m_points[2].m_x, 10.0);
	}

	TEST(triangle, sort_3)
	{
		std::vector<vector::Vector2d> points{
			{1.0, 2.0},
			{ 10.0, 3.0 },
			{ 10.0, 5.0 },

		};
		geo::Triangle triangle{
			points
		};
		triangle.sort();
		EXPECT_EQ(triangle.m_points[0].m_y, 2.0);
		EXPECT_EQ(triangle.m_points[0].m_x, 1.0);
		EXPECT_EQ(triangle.m_points[1].m_y, 3.0);
		EXPECT_EQ(triangle.m_points[1].m_x, 10.0);
		EXPECT_EQ(triangle.m_points[2].m_y, 5.0);
		EXPECT_EQ(triangle.m_points[2].m_x, 10.0);
	}
}