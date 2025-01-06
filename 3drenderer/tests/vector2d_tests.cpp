#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../vector2d.h"
#include "../vector3d.h"

namespace vector2d_tests
{
	
	TEST(vector2d, vars)
	{
		vector::Vector2d v2d{ 1.0, 1.0 };
		EXPECT_EQ(v2d.m_x, 1.0);
		EXPECT_EQ(v2d.m_y, 1.0);
	}

	TEST(vector2d, get_magnitude)
	{
		vector::Vector2d v2d{ 3.0, 4.0 };
		EXPECT_EQ(v2d.get_magnitude(), 5.0);
	}

	TEST(vector2d, operator_plus_equal)
	{
		vector::Vector2d v2d{ 3.0, 4.0 };
		vector::Vector2d v2d2{ 1.0, 1.0 };
		v2d += v2d2;
		EXPECT_EQ(v2d.m_x, 4.0);
		EXPECT_EQ(v2d.m_y, 5.0);
	}

	TEST(vector2d, operator_plus)
	{
		vector::Vector2d v2d{ 3.0, 4.0 };
		vector::Vector2d v2d2{ 1.0, 1.0 };
		vector::Vector2d v2d3 = v2d + v2d2;
		EXPECT_EQ(v2d3.m_x, 4.0);
		EXPECT_EQ(v2d3.m_y, 5.0);
	}

	TEST(vector2d, operator_minus_equal)
	{
		vector::Vector2d v2d{ 3.0, 4.0 };
		vector::Vector2d v2d2{ 1.0, 1.0 };
		v2d -= v2d2;
		EXPECT_EQ(v2d.m_x, 2.0);
		EXPECT_EQ(v2d.m_y, 3.0);
	}

	TEST(vector2d, operator_minus)
	{
		vector::Vector2d v2d{ 3.0, 4.0 };
		vector::Vector2d v2d2{ 1.0, 1.0 };
		vector::Vector2d v2d3 = v2d - v2d2;
		EXPECT_EQ(v2d3.m_x, 2.0);
		EXPECT_EQ(v2d3.m_y, 3.0);
	}

	TEST(vector2d, operator_mult_equal)
	{
		vector::Vector2d v2d{ 3.0, 4.0 };
		v2d *= 2.0;
		EXPECT_EQ(v2d.m_x, 6.0);
		EXPECT_EQ(v2d.m_y, 8.0);
	}

	TEST(vector2d, operator_div_equal)
	{
		vector::Vector2d v2d{ 2.0, 4.0 };
		v2d /= 2.0;
		EXPECT_EQ(v2d.m_x, 1.0);
		EXPECT_EQ(v2d.m_y, 2.0);
	}

	TEST(vector2d, normalize)
	{
		vector::Vector2d v2d{ 3.0, 4.0 };
		v2d.normalize();
		EXPECT_EQ(v2d.m_x, 3.0/5.0);
		EXPECT_EQ(v2d.m_y, 4.0/5.0);
	}

	TEST(vector2d, dot_product)
	{
		vector::Vector2d v2d{ 2.0, 4.0 };
		vector::Vector2d v2d2{ 1.0, 1.0 };
		EXPECT_EQ(v2d.dot_product(v2d2), 6.0);
	}

	TEST(vector2d, rotate)
	{
		vector::Vector2d v2d{ 1.0, 1.0 };
		double angle{ 90 };
		v2d.rotate(90.0);
		EXPECT_EQ(v2d.m_x, (1.0 * cos(angle) - 1.0 * sin(angle)));
		EXPECT_EQ(v2d.m_y, (1.0 * sin(angle) + 1.0 * cos(angle)));
	}

}