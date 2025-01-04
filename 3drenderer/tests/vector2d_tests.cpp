#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../vector2d.h"
#include "../vector3d.h"

namespace vector2d_tests
{
	
	TEST(vector2d, xAndy)
	{
		vector::Vector2d v2d{ 1.0, 1.0 };
		EXPECT_EQ(v2d.m_x, 1.0);
		EXPECT_EQ(v2d.m_y, 1.0);
	}

}