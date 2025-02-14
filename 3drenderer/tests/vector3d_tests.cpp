#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../vector3d.h"
#include "../vector4d.h"
#include "../vector2d.h"
#include <math.h>

namespace vector3d_tests
{
	TEST(vector3d, vars)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		EXPECT_EQ(v3d.m_x, 1.0);
		EXPECT_EQ(v3d.m_y, 1.0);
		EXPECT_EQ(v3d.m_z, 1.0);
	}

	TEST(vector3d, cosntruct_with_vec4d)
	{
		vector::Vector4d v4d{ 1.0, 1.0, 1.0, 1.0 };
		vector::Vector3d v3d{ v4d };
		EXPECT_EQ(v3d.m_x, 1.0);
		EXPECT_EQ(v3d.m_y, 1.0);
		EXPECT_EQ(v3d.m_z, 1.0);
	}

	TEST(vector3d, get_magnitude)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		EXPECT_EQ(v3d.get_magnitude(), 3.0);
	}

	TEST(vector3d, operator_plus_equal)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		vector::Vector3d v3d2{ 3.0, 4.0, 5.0 };
		v3d += v3d2;
		EXPECT_EQ(v3d.m_x, 4.0);
		EXPECT_EQ(v3d.m_y, 6.0);
		EXPECT_EQ(v3d.m_z, 7.0);
	}

	TEST(vector3d, operator_plus)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		vector::Vector3d v3d2{ 3.0, 4.0, 5.0 };
		vector::Vector3d v3d3 =  v3d + v3d2;
		EXPECT_EQ(v3d3.m_x, 4.0);
		EXPECT_EQ(v3d3.m_y, 6.0);
		EXPECT_EQ(v3d3.m_z, 7.0);
	}

	TEST(vector3d, operator_minus_equal)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		vector::Vector3d v3d2{ 3.0, 4.0, 5.0 };
		v3d -= v3d2;
		EXPECT_EQ(v3d.m_x, -2.0);
		EXPECT_EQ(v3d.m_y, -2.0);
		EXPECT_EQ(v3d.m_z, -3.0);
	}

	TEST(vector3d, operator_minus)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		vector::Vector3d v3d2{ 3.0, 4.0, 5.0 };
		vector::Vector3d v3d3 = v3d - v3d2;
		EXPECT_EQ(v3d3.m_x, -2.0);
		EXPECT_EQ(v3d3.m_y, -2.0);
		EXPECT_EQ(v3d3.m_z, -3.0);
	}

	TEST(vector3d, operator_mult_equal)
	{
		vector::Vector3d v3d{ 3.0, 4.0, 5.0 };
		v3d *= 2.0;
		EXPECT_EQ(v3d.m_x, 6.0);
		EXPECT_EQ(v3d.m_y, 8.0);
		EXPECT_EQ(v3d.m_z, 10.0);
	}

	TEST(vector3d, operator_div_equal)
	{
		vector::Vector3d v3d{ 2.0, 4.0, 6.0 };
		v3d /= 2.0;
		EXPECT_EQ(v3d.m_x, 1.0);
		EXPECT_EQ(v3d.m_y, 2.0);
		EXPECT_EQ(v3d.m_z, 3.0);
	}

	TEST(vector3d, normalize)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		v3d.normalize();
		EXPECT_EQ(v3d.m_x, 1.0 / 3.0);
		EXPECT_EQ(v3d.m_y, 2.0 / 3.0);
		EXPECT_EQ(v3d.m_z, 2.0 / 3.0);
	}

	TEST(vector3d, cross_product)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		vector::Vector3d v3d2{ 3.0, 4.0, 5.0 };
		vector::Vector3d v3d3 = v3d.cross_product(v3d2);
		EXPECT_EQ(v3d3.m_x, 2.0);
		EXPECT_EQ(v3d3.m_y, 1.0);
		EXPECT_EQ(v3d3.m_z, -2.0);
	}

	TEST(vector3d, dot_product)
	{
		vector::Vector3d v3d{ 1.0, 2.0, 2.0 };
		vector::Vector3d v3d2{ 3.0, 4.0, 5.0 };
		EXPECT_EQ(v3d.dot_product(v3d2), 21.0);
	}

	TEST(vector3d, rotate_x)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		double angle{ 90 };
		v3d.rotate_x(90.0);
		EXPECT_EQ(v3d.m_x, 1.0);
		EXPECT_EQ(v3d.m_y, (1.0 * cos(angle) - 1.0 * sin(angle)));
		EXPECT_EQ(v3d.m_z, (1.0 * sin(angle) + 1.0 * cos(angle)));
	}

	TEST(vector3d, rotate_y)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		double angle{ 90 };
		v3d.rotate_y(90.0);
		EXPECT_EQ(v3d.m_x, (1.0 * cos(angle) - 1.0 * sin(angle)));
		EXPECT_EQ(v3d.m_y, 1.0);
		EXPECT_EQ(v3d.m_z, (1.0 * sin(angle) + 1.0 * cos(angle)));
	}

	TEST(vector3d, rotate_z)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		double angle{ 90 };
		v3d.rotate_z(90.0);
		EXPECT_EQ(v3d.m_x, (1.0 * cos(angle) - 1.0 * sin(angle)));
		EXPECT_EQ(v3d.m_y, (1.0 * sin(angle) + 1.0 * cos(angle)));
		EXPECT_EQ(v3d.m_z, 1.0);
	}

	TEST(vector3d, project)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		vector::Vector2d v2d{ v3d.project(90) };
		EXPECT_EQ(v2d.m_x, 90.0);
		EXPECT_EQ(v2d.m_y, 90.0);
	}
}