#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../vector3d.h"
#include "../vector2d.h"
#include <math.h>

namespace vector3d_tests
{
	TEST(vector3d, getters)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		EXPECT_EQ(v3d.get_x(), 1.0);
		EXPECT_EQ(v3d.get_y(), 1.0);
		EXPECT_EQ(v3d.get_z(), 1.0);
	}

	TEST(vector3d, setters)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		v3d.set_x(2.0);
		v3d.set_y(2.0);
		v3d.set_z(2.0);
		EXPECT_EQ(v3d.get_x(), 2.0);
		EXPECT_EQ(v3d.get_y(), 2.0);
		EXPECT_EQ(v3d.get_z(), 2.0);
	}

	TEST(vector3d, rotate_x)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		double angle{ 90 };
		v3d.rotate_x(90.0);
		EXPECT_EQ(v3d.get_x(), 1.0);
		EXPECT_EQ(v3d.get_y(), (1.0 * cos(angle) - 1.0 * sin(angle)));
		EXPECT_EQ(v3d.get_z(), (1.0 * sin(angle) + 1.0 * cos(angle)));
	}

	TEST(vector3d, rotate_y)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		double angle{ 90 };
		v3d.rotate_y(90.0);
		EXPECT_EQ(v3d.get_x(), (1.0 * cos(angle) - 1.0 * sin(angle)));
		EXPECT_EQ(v3d.get_y(), 1.0);
		EXPECT_EQ(v3d.get_z(), (1.0 * sin(angle) + 1.0 * cos(angle)));
	}

	TEST(vector3d, rotate_z)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		double angle{ 90 };
		v3d.rotate_z(90.0);
		EXPECT_EQ(v3d.get_x(), (1.0 * cos(angle) - 1.0 * sin(angle)));
		EXPECT_EQ(v3d.get_y(), (1.0 * sin(angle) + 1.0 * cos(angle)));
		EXPECT_EQ(v3d.get_z(), 1.0);
	}

	TEST(vector3d, project)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		vector::Vector2d v2d{ v3d.project(90) };
		EXPECT_EQ(v2d.get_x(), 90.0);
		EXPECT_EQ(v2d.get_y(), 90.0);
	}
}