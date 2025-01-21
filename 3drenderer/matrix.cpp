#include <math.h>
#include "matrix.h"

namespace matrix
{
	Matrix4x4::Matrix4x4()
		: m_matrix{
			{ 1.0, 0, 0, 0},
			{ 0, 1.0, 0, 0},
			{ 0, 0, 1.0, 0},
			{ 0, 0, 0, 1.0},
		}
	{
	};

	Matrix4x4 Matrix4x4::make_scale_matrix(const double scale_x, const double scale_y, const double scale_z)
	{
		matrix::Matrix4x4 scale_matrix{};
		scale_matrix.m_matrix[0][0] = scale_x;
		scale_matrix.m_matrix[1][1] = scale_y;
		scale_matrix.m_matrix[2][2] = scale_z;
		return scale_matrix;
	}

	Matrix4x4 Matrix4x4::make_translation_matrix(const double transalation_x, const double transalation_y, const double transalation_z)
	{
		matrix::Matrix4x4 translation_matrix{};
		translation_matrix.m_matrix[0][3] = transalation_x;
		translation_matrix.m_matrix[1][3] = transalation_y;
		translation_matrix.m_matrix[2][3] = transalation_z;
		return translation_matrix;
	}

	Matrix4x4 Matrix4x4::make_rotation_matrix(const double angle, const char axis)
	{
		matrix::Matrix4x4 rotation_matrix{};
		switch (axis)
		{
		case('x'):
			rotation_matrix.m_matrix[1][1] = cos(angle);
			rotation_matrix.m_matrix[1][2] = -sin(angle);
			rotation_matrix.m_matrix[2][1] = sin(angle);
			rotation_matrix.m_matrix[2][2] = cos(angle);
			break;
		case('y'):
			rotation_matrix.m_matrix[0][0] = cos(angle);
			rotation_matrix.m_matrix[0][2] = sin(angle);
			rotation_matrix.m_matrix[2][0] = -sin(angle);
			rotation_matrix.m_matrix[2][2] = cos(angle);
			break;
		case('z'):
			rotation_matrix.m_matrix[0][0] = cos(angle);
			rotation_matrix.m_matrix[0][1] = -sin(angle);
			rotation_matrix.m_matrix[1][0] = sin(angle);
			rotation_matrix.m_matrix[1][1] = cos(angle);
			break;
		default:
			break;
		}
		return rotation_matrix;
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		for (std::size_t i{ 0 }; i < 4; i++)
		{
			for (std::size_t j{ 0 }; j < 4; j++)
			{
				//this->m_matrix[i][j] = this->m_matrix[i][0] * m.m_matrix[0][j] + this->m_matrix[i][1] * m.m_matrix[1][j] + this->m_matrix[i][2] * m.m_matrix[2][j] + this->m_matrix[i][3] * m.m_matrix[3][j];
				this->m_matrix[i][j] = m.m_matrix[i][0] * this->m_matrix[0][j] + m.m_matrix[i][1] * this->m_matrix[1][j] + m.m_matrix[i][2] * this->m_matrix[2][j] + m.m_matrix[i][3] * this->m_matrix[3][j];
			}
		}
		return *this;
	}

	Matrix4x4 Matrix4x4::mult_matrix(const Matrix4x4& a, const Matrix4x4& b)
	{
		matrix::Matrix4x4 result{};
		for (std::size_t i{ 0 }; i < 4; i++)
		{
			for (std::size_t j{ 0 }; j < 4; j++)
			{
				result.m_matrix[i][j] = a.m_matrix[i][0] * b.m_matrix[0][j] + a.m_matrix[i][1] * b.m_matrix[1][j] + a.m_matrix[i][2] * b.m_matrix[2][j] + a.m_matrix[i][3] * b.m_matrix[3][j];
			}
		}
		return result;
	}

	vector::Vector4d Matrix4x4::mult_vec4d(const vector::Vector4d& vec4d) const
	{
		return vector::Vector4d{
			vec4d.m_x * m_matrix[0][0] + vec4d.m_y * m_matrix[0][1] + vec4d.m_z * m_matrix[0][2] + vec4d.m_w * m_matrix[0][3],
			vec4d.m_x * m_matrix[1][0] + vec4d.m_y * m_matrix[1][1] + vec4d.m_z * m_matrix[1][2] + vec4d.m_w * m_matrix[1][3],
			vec4d.m_x * m_matrix[2][0] + vec4d.m_y * m_matrix[2][1] + vec4d.m_z * m_matrix[2][2] + vec4d.m_w * m_matrix[2][3],
			vec4d.m_x * m_matrix[3][0] + vec4d.m_y * m_matrix[3][1] + vec4d.m_z * m_matrix[3][2] + vec4d.m_w * m_matrix[3][3]
		};
	}
}