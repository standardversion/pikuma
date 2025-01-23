#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../matrix.h"
#include "../vector4d.h"
#include <math.h>

namespace matrix_tests
{
	TEST(matrix, identity_matrix)
	{
		matrix::Matrix4x4 m{};
		EXPECT_EQ(m.m_matrix[0][0], 1.0);
		EXPECT_EQ(m.m_matrix[0][1], 0.0);
		EXPECT_EQ(m.m_matrix[0][2], 0.0);
		EXPECT_EQ(m.m_matrix[0][3], 0.0);

		EXPECT_EQ(m.m_matrix[1][0], 0.0);
		EXPECT_EQ(m.m_matrix[1][1], 1.0);
		EXPECT_EQ(m.m_matrix[1][2], 0.0);
		EXPECT_EQ(m.m_matrix[1][3], 0.0);

		EXPECT_EQ(m.m_matrix[2][0], 0.0);
		EXPECT_EQ(m.m_matrix[2][1], 0.0);
		EXPECT_EQ(m.m_matrix[2][2], 1.0);
		EXPECT_EQ(m.m_matrix[2][3], 0.0);

		EXPECT_EQ(m.m_matrix[3][0], 0.0);
		EXPECT_EQ(m.m_matrix[3][1], 0.0);
		EXPECT_EQ(m.m_matrix[3][2], 0.0);
		EXPECT_EQ(m.m_matrix[3][3], 1.0);
	}

	TEST(matrix, projection_matrix)
	{
		matrix::Matrix4x4 m{1.5708, 1.0, 1.0, 10.0};
		EXPECT_NEAR(m.m_matrix[0][0], 1.0, 0.01);
		EXPECT_EQ(m.m_matrix[0][1], 0.0);
		EXPECT_EQ(m.m_matrix[0][2], 0.0);
		EXPECT_EQ(m.m_matrix[0][3], 0.0);

		EXPECT_EQ(m.m_matrix[1][0], 0.0);
		EXPECT_NEAR(m.m_matrix[1][1], 1.0, 0.01);
		EXPECT_EQ(m.m_matrix[1][2], 0.0);
		EXPECT_EQ(m.m_matrix[1][3], 0.0);

		EXPECT_EQ(m.m_matrix[2][0], 0.0);
		EXPECT_EQ(m.m_matrix[2][1], 0.0);
		EXPECT_NEAR(m.m_matrix[2][2], 1.11111, 0.01);
		EXPECT_NEAR(m.m_matrix[2][3], -1.11111, 0.01);

		EXPECT_EQ(m.m_matrix[3][0], 0.0);
		EXPECT_EQ(m.m_matrix[3][1], 0.0);
		EXPECT_EQ(m.m_matrix[3][2], 1.0);
		EXPECT_EQ(m.m_matrix[3][3], 0.0);
	}

	TEST(matrix, make_scale_matrix)
	{
		matrix::Matrix4x4 m{ matrix::Matrix4x4::make_scale_matrix(3.0, 4.0, 5.0)};
		EXPECT_EQ(m.m_matrix[0][0], 3.0);
		EXPECT_EQ(m.m_matrix[0][1], 0.0);
		EXPECT_EQ(m.m_matrix[0][2], 0.0);
		EXPECT_EQ(m.m_matrix[0][3], 0.0);

		EXPECT_EQ(m.m_matrix[1][0], 0.0);
		EXPECT_EQ(m.m_matrix[1][1], 4.0);
		EXPECT_EQ(m.m_matrix[1][2], 0.0);
		EXPECT_EQ(m.m_matrix[1][3], 0.0);

		EXPECT_EQ(m.m_matrix[2][0], 0.0);
		EXPECT_EQ(m.m_matrix[2][1], 0.0);
		EXPECT_EQ(m.m_matrix[2][2], 5.0);
		EXPECT_EQ(m.m_matrix[2][3], 0.0);

		EXPECT_EQ(m.m_matrix[3][0], 0.0);
		EXPECT_EQ(m.m_matrix[3][1], 0.0);
		EXPECT_EQ(m.m_matrix[3][2], 0.0);
		EXPECT_EQ(m.m_matrix[3][3], 1.0);
	}

	TEST(matrix, make_trasnlation_matrix)
	{
		matrix::Matrix4x4 m{ matrix::Matrix4x4::make_translation_matrix(3.0, 4.0, 5.0) };
		EXPECT_EQ(m.m_matrix[0][0], 1.0);
		EXPECT_EQ(m.m_matrix[0][1], 0.0);
		EXPECT_EQ(m.m_matrix[0][2], 0.0);
		EXPECT_EQ(m.m_matrix[0][3], 3.0);

		EXPECT_EQ(m.m_matrix[1][0], 0.0);
		EXPECT_EQ(m.m_matrix[1][1], 1.0);
		EXPECT_EQ(m.m_matrix[1][2], 0.0);
		EXPECT_EQ(m.m_matrix[1][3], 4.0);

		EXPECT_EQ(m.m_matrix[2][0], 0.0);
		EXPECT_EQ(m.m_matrix[2][1], 0.0);
		EXPECT_EQ(m.m_matrix[2][2], 1.0);
		EXPECT_EQ(m.m_matrix[2][3], 5.0);

		EXPECT_EQ(m.m_matrix[3][0], 0.0);
		EXPECT_EQ(m.m_matrix[3][1], 0.0);
		EXPECT_EQ(m.m_matrix[3][2], 0.0);
		EXPECT_EQ(m.m_matrix[3][3], 1.0);
	}

	TEST(matrix, make_rotation_matrix_x)
	{
		constexpr const double angle{ 1.5708 };
		matrix::Matrix4x4 m{ matrix::Matrix4x4::make_rotation_matrix(angle, 'x')};
		EXPECT_EQ(m.m_matrix[0][0], 1.0);
		EXPECT_EQ(m.m_matrix[0][1], 0.0);
		EXPECT_EQ(m.m_matrix[0][2], 0.0);
		EXPECT_EQ(m.m_matrix[0][3], 0.0);

		EXPECT_EQ(m.m_matrix[1][0], 0.0);
		EXPECT_EQ(m.m_matrix[1][1], cos(angle));
		EXPECT_EQ(m.m_matrix[1][2], -sin(angle));
		EXPECT_EQ(m.m_matrix[1][3], 0.0);

		EXPECT_EQ(m.m_matrix[2][0], 0.0);
		EXPECT_EQ(m.m_matrix[2][1], sin(angle));
		EXPECT_EQ(m.m_matrix[2][2], cos(angle));
		EXPECT_EQ(m.m_matrix[2][3], 0.0);

		EXPECT_EQ(m.m_matrix[3][0], 0.0);
		EXPECT_EQ(m.m_matrix[3][1], 0.0);
		EXPECT_EQ(m.m_matrix[3][2], 0.0);
		EXPECT_EQ(m.m_matrix[3][3], 1.0);
	}

	TEST(matrix, make_rotation_matrix_y)
	{
		constexpr const double angle{ 1.5708 };
		matrix::Matrix4x4 m{ matrix::Matrix4x4::make_rotation_matrix(angle, 'y') };
		EXPECT_EQ(m.m_matrix[0][0], cos(angle));
		EXPECT_EQ(m.m_matrix[0][1], 0.0);
		EXPECT_EQ(m.m_matrix[0][2], sin(angle));
		EXPECT_EQ(m.m_matrix[0][3], 0.0);

		EXPECT_EQ(m.m_matrix[1][0], 0.0);
		EXPECT_EQ(m.m_matrix[1][1], 1.0);
		EXPECT_EQ(m.m_matrix[1][2], 0.0);
		EXPECT_EQ(m.m_matrix[1][3], 0.0);

		EXPECT_EQ(m.m_matrix[2][0], -sin(angle));
		EXPECT_EQ(m.m_matrix[2][1], 0.0);
		EXPECT_EQ(m.m_matrix[2][2], cos(angle));
		EXPECT_EQ(m.m_matrix[2][3], 0.0);

		EXPECT_EQ(m.m_matrix[3][0], 0.0);
		EXPECT_EQ(m.m_matrix[3][1], 0.0);
		EXPECT_EQ(m.m_matrix[3][2], 0.0);
		EXPECT_EQ(m.m_matrix[3][3], 1.0);
	}

	TEST(matrix, make_rotation_matrix_z)
	{
		constexpr const double angle{ 1.5708 };
		matrix::Matrix4x4 m{ matrix::Matrix4x4::make_rotation_matrix(angle, 'z') };
		EXPECT_EQ(m.m_matrix[0][0], cos(angle));
		EXPECT_EQ(m.m_matrix[0][1], -sin(angle));
		EXPECT_EQ(m.m_matrix[0][2], 0.0);
		EXPECT_EQ(m.m_matrix[0][3], 0.0);

		EXPECT_EQ(m.m_matrix[1][0], sin(angle));
		EXPECT_EQ(m.m_matrix[1][1], cos(angle));
		EXPECT_EQ(m.m_matrix[1][2], 0.0);
		EXPECT_EQ(m.m_matrix[1][3], 0.0);

		EXPECT_EQ(m.m_matrix[2][0], 0.0);
		EXPECT_EQ(m.m_matrix[2][1], 0.0);
		EXPECT_EQ(m.m_matrix[2][2], 1.0);
		EXPECT_EQ(m.m_matrix[2][3], 0.0);

		EXPECT_EQ(m.m_matrix[3][0], 0.0);
		EXPECT_EQ(m.m_matrix[3][1], 0.0);
		EXPECT_EQ(m.m_matrix[3][2], 0.0);
		EXPECT_EQ(m.m_matrix[3][3], 1.0);
	}

	TEST(matrix, operator_mult_eq)
	{
		matrix::Matrix4x4 m{};
		m.m_matrix[0][0] = 1;
		m.m_matrix[0][1] = 2;
		m.m_matrix[0][2] = 3;
		m.m_matrix[0][3] = 4;

		m.m_matrix[1][0] = 5;
		m.m_matrix[1][1] = 6;
		m.m_matrix[1][2] = 7;
		m.m_matrix[1][3] = 8;

		m.m_matrix[2][0] = 9;
		m.m_matrix[2][1] = 10;
		m.m_matrix[2][2] = 11;
		m.m_matrix[2][3] = 12;

		m.m_matrix[3][0] = 13;
		m.m_matrix[3][1] = 14;
		m.m_matrix[3][2] = 15;
		m.m_matrix[3][3] = 16;

		matrix::Matrix4x4 m2{};
		m2.m_matrix[0][0] = 16;
		m2.m_matrix[0][1] = 15;
		m2.m_matrix[0][2] = 14;
		m2.m_matrix[0][3] = 13;

		m2.m_matrix[1][0] = 12;
		m2.m_matrix[1][1] = 11;
		m2.m_matrix[1][2] = 10;
		m2.m_matrix[1][3] = 9;

		m2.m_matrix[2][0] = 8;
		m2.m_matrix[2][1] = 7;
		m2.m_matrix[2][2] = 6;
		m2.m_matrix[2][3] = 5;

		m2.m_matrix[3][0] = 4;
		m2.m_matrix[3][1] = 3;
		m2.m_matrix[3][2] = 2;
		m2.m_matrix[3][3] = 1;

		m2 *= m;

		EXPECT_EQ(m2.m_matrix[0][0], 80);
		EXPECT_EQ(m2.m_matrix[0][1], 70);
		EXPECT_EQ(m2.m_matrix[0][2], 60);
		EXPECT_EQ(m2.m_matrix[0][3], 50);

		EXPECT_EQ(m2.m_matrix[1][0], 240);
		EXPECT_EQ(m2.m_matrix[1][1], 214);
		EXPECT_EQ(m2.m_matrix[1][2], 188);
		EXPECT_EQ(m2.m_matrix[1][3], 162);

		EXPECT_EQ(m2.m_matrix[2][0], 400);
		EXPECT_EQ(m2.m_matrix[2][1], 358);
		EXPECT_EQ(m2.m_matrix[2][2], 316);
		EXPECT_EQ(m2.m_matrix[2][3], 274);

		EXPECT_EQ(m2.m_matrix[3][0], 560);
		EXPECT_EQ(m2.m_matrix[3][1], 502);
		EXPECT_EQ(m2.m_matrix[3][2], 444);
		EXPECT_EQ(m2.m_matrix[3][3], 386);
	}

	TEST(matrix, mult_vec4d)
	{
		matrix::Matrix4x4 m{};
		m.m_matrix[0][0] = 1;
		m.m_matrix[0][1] = 2;
		m.m_matrix[0][2] = 3;
		m.m_matrix[0][3] = 4;

		m.m_matrix[1][0] = 5;
		m.m_matrix[1][1] = 6;
		m.m_matrix[1][2] = 7;
		m.m_matrix[1][3] = 8;

		m.m_matrix[2][0] = 9;
		m.m_matrix[2][1] = 10;
		m.m_matrix[2][2] = 11;
		m.m_matrix[2][3] = 12;

		m.m_matrix[3][0] = 13;
		m.m_matrix[3][1] = 14;
		m.m_matrix[3][2] = 15;
		m.m_matrix[3][3] = 16;

		vector::Vector4d vec4d{ 5, 10, 1, 2 };
		vector::Vector4d r{ m.mult_vec4d(vec4d) };

		EXPECT_EQ(r.m_x, 36);
		EXPECT_EQ(r.m_y, 108);
		EXPECT_EQ(r.m_z, 180);
		EXPECT_EQ(r.m_w, 252);
	}

	TEST(matrix, project)
	{
		matrix::Matrix4x4 m{};
		m.m_matrix[0][0] = 1;
		m.m_matrix[0][1] = 2;
		m.m_matrix[0][2] = 3;
		m.m_matrix[0][3] = 4;

		m.m_matrix[1][0] = 5;
		m.m_matrix[1][1] = 6;
		m.m_matrix[1][2] = 7;
		m.m_matrix[1][3] = 8;

		m.m_matrix[2][0] = 9;
		m.m_matrix[2][1] = 10;
		m.m_matrix[2][2] = 11;
		m.m_matrix[2][3] = 12;

		m.m_matrix[3][0] = 13;
		m.m_matrix[3][1] = 14;
		m.m_matrix[3][2] = 15;
		m.m_matrix[3][3] = 16;

		vector::Vector4d vec4d{ 5, 10, 1, 2 };
		vector::Vector4d r{ m.project(vec4d) };

		EXPECT_NEAR(r.m_x, 0.1428, 0.01);
		EXPECT_NEAR(r.m_y, 0.4285, 0.01);
		EXPECT_NEAR(r.m_z, 0.7142, 0.01);
		EXPECT_EQ(r.m_w, 252);
	}

	TEST(matrix, project_m_w_0)
	{
		vector::Vector4d vec4d{ 90, 108, 126, 144 };
		vector::Vector4d vec4d_r{ 90, 108, 126, 0 };
		class MockMatrix : public matrix::Matrix4x4
		{
		public:
			MOCK_METHOD(vector::Vector4d, mult_vec4d, (const vector::Vector4d& vec4d), (const, override));
		};
		MockMatrix m{};
		m.m_matrix[0][0] = 1;
		m.m_matrix[0][1] = 2;
		m.m_matrix[0][2] = 3;
		m.m_matrix[0][3] = 4;

		m.m_matrix[1][0] = 5;
		m.m_matrix[1][1] = 6;
		m.m_matrix[1][2] = 7;
		m.m_matrix[1][3] = 8;

		m.m_matrix[2][0] = 9;
		m.m_matrix[2][1] = 10;
		m.m_matrix[2][2] = 11;
		m.m_matrix[2][3] = 12;

		m.m_matrix[3][0] = 13;
		m.m_matrix[3][1] = 14;
		m.m_matrix[3][2] = 15;
		m.m_matrix[3][3] = 16;
		EXPECT_CALL(m, mult_vec4d(::testing::_))
			.WillOnce(::testing::Return(vec4d_r));

		vector::Vector4d r{ m.project(vec4d) };

		EXPECT_EQ(r.m_x, 90);
		EXPECT_EQ(r.m_y, 108);
		EXPECT_EQ(r.m_z, 126);
		EXPECT_EQ(r.m_w, 0);
	}
}