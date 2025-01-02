#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../vector2d.h"
#include "../vector3d.h"

namespace vector2d_tests
{
	
	TEST(vector2d, getters)
	{
		vector::Vector2d v2d{ 1.0, 1.0 };
		EXPECT_EQ(v2d.get_x(), 1.0);
		EXPECT_EQ(v2d.get_y(), 1.0);
	}

	TEST(vector2d, setters)
	{
		vector::Vector2d v2d{ 1.0, 1.0 };
		v2d.set_x(2.0);
		v2d.set_y(2.0);
		EXPECT_EQ(v2d.get_x(), 2.0);
		EXPECT_EQ(v2d.get_y(), 2.0);
	}
}