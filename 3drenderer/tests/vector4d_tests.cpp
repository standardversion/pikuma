#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../vector3d.h"
#include "../vector4d.h"
#include <math.h>

namespace vector4d_tests
{
	TEST(vector4d, vars)
	{
		vector::Vector4d v4d{ 1.0, 1.0, 1.0, 1.0 };
		EXPECT_EQ(v4d.m_x, 1.0);
		EXPECT_EQ(v4d.m_y, 1.0);
		EXPECT_EQ(v4d.m_z, 1.0);
		EXPECT_EQ(v4d.m_w, 1.0);
	}

	TEST(vector4d, cosntruct_with_vec3d)
	{
		vector::Vector3d v3d{ 1.0, 1.0, 1.0 };
		vector::Vector4d v4d{ v3d };
		EXPECT_EQ(v4d.m_x, 1.0);
		EXPECT_EQ(v4d.m_y, 1.0);
		EXPECT_EQ(v4d.m_z, 1.0);
		EXPECT_EQ(v4d.m_w, 1.0);
	}

}