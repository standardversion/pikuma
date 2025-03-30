#include <math.h>
#include "matrix.h"

namespace matrix
{
	// | 1 0 0 0 |
	// | 0 1 0 0 |
	// | 0 0 1 0 |
	// | 0 0 0 1 |
	Matrix4x4::Matrix4x4()
		: m_matrix{
			{ 1.0, 0, 0, 0},
			{ 0, 1.0, 0, 0},
			{ 0, 0, 1.0, 0},
			{ 0, 0, 0, 1.0},
		}
	{
	};

	Matrix4x4::Matrix4x4(const std::vector<double>& v1, const std::vector<double>& v2, const std::vector<double>& v3, const std::vector<double>& v4)
		: m_matrix{
			v1,
			v2,
			v3,
			v4
		}
	{
	}

	// | (h/w)*1/tan(fov/2)             0              0                 0 |
	// |                  0  1/tan(fov/2)              0                 0 |
	// |                  0             0     zf/(zf-zn)  (-zf*zn)/(zf-zn) |
	// |                  0             0              1                 0 |
	Matrix4x4::Matrix4x4(const double fov, const double aspect, const double znear, const double zfar)
		: m_matrix{
			{ 1.0, 0, 0, 0},
			{ 0, 1.0, 0, 0},
			{ 0, 0, 1.0, 0},
			{ 0, 0, 0, 1.0},
		}
	{
		m_matrix[0][0] = aspect * (1 / tan(fov / 2.0));
		m_matrix[1][1] = 1 / tan(fov / 2.0);
		m_matrix[2][2] = zfar / (zfar - znear);
		m_matrix[2][3] = (-zfar * znear) / (zfar - znear);
		m_matrix[3][2] = 1.0;
		m_matrix[3][3] = 0.0;
	}

	// Get the determinant of the 3x3 submatrix after removing row 'r' and column 'c'
	double Matrix4x4::get_submatrix_determinant(int r, int c) const
	{
		Matrix3x3 submatrix;
		int sub_row = 0, sub_col;

		for (int i = 0; i < 4; i++) {
			if (i == r) continue;  // Skip row 'r'
			sub_col = 0;
			for (int j = 0; j < 4; j++) {
				if (j == c) continue;  // Skip column 'c'
				submatrix.m_matrix[sub_row][sub_col] = m_matrix[i][j];
				sub_col++;
			}
			sub_row++;
		}

		return submatrix.get_determinant();
	}

	// Calculate the cofactor for element at row 'r' and column 'c'
	double Matrix4x4::get_cofactor(int r, int c) const
	{
		double minor = get_submatrix_determinant(r, c);
		return ((r + c) % 2 == 0) ? minor : -minor;
	}

	Matrix4x4 Matrix4x4::get_transpose() const
	{
		matrix::Matrix4x4 transpose{};
		/*transpose.m_matrix[0][0] = m_matrix[0][0];
		transpose.m_matrix[0][1] = m_matrix[1][0];
		transpose.m_matrix[0][2] = m_matrix[2][0];
		transpose.m_matrix[0][3] = m_matrix[3][0];

		transpose.m_matrix[1][0] = m_matrix[0][1];
		transpose.m_matrix[1][1] = m_matrix[1][1];
		transpose.m_matrix[1][2] = m_matrix[2][1];
		transpose.m_matrix[1][3] = m_matrix[3][1];

		transpose.m_matrix[2][0] = m_matrix[0][2];
		transpose.m_matrix[2][1] = m_matrix[1][2];
		transpose.m_matrix[2][2] = m_matrix[2][2];
		transpose.m_matrix[2][3] = m_matrix[3][2];

		transpose.m_matrix[3][0] = m_matrix[0][3];
		transpose.m_matrix[3][1] = m_matrix[1][3];
		transpose.m_matrix[3][2] = m_matrix[2][3];
		transpose.m_matrix[3][3] = m_matrix[3][3];*/

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				transpose.m_matrix[i][j] = m_matrix[j][i];
			}
		}

		return transpose;
	}

	// | sx  0  0  0 |
	// |  0 sy  0  0 |
	// |  0  0 sz  0 |
	// |  0  0  0  1 |
	Matrix4x4 Matrix4x4::make_scale_matrix(const double scale_x, const double scale_y, const double scale_z)
	{
		matrix::Matrix4x4 scale_matrix{};
		scale_matrix.m_matrix[0][0] = scale_x;
		scale_matrix.m_matrix[1][1] = scale_y;
		scale_matrix.m_matrix[2][2] = scale_z;
		return scale_matrix;
	}

	// | 1  0  0  tx |
	// | 0  1  0  ty |
	// | 0  0  1  tz |
	// | 0  0  0  1  |
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
			// | 1  0  0  0 |
			// | 0  c -s  0 |
			// | 0  s  c  0 |
			// | 0  0  0  1 |
			rotation_matrix.m_matrix[1][1] = cos(angle);
			rotation_matrix.m_matrix[1][2] = -sin(angle);
			rotation_matrix.m_matrix[2][1] = sin(angle);
			rotation_matrix.m_matrix[2][2] = cos(angle);
			break;
		case('y'):
			// |  c  0  s  0 |
			// |  0  1  0  0 |
			// | -s  0  c  0 |
			// |  0  0  0  1 |
			rotation_matrix.m_matrix[0][0] = cos(angle);
			rotation_matrix.m_matrix[0][2] = sin(angle);
			rotation_matrix.m_matrix[2][0] = -sin(angle);
			rotation_matrix.m_matrix[2][2] = cos(angle);
			break;
		case('z'):
			// | c -s  0  0 |
			// | s  c  0  0 |
			// | 0  0  1  0 |
			// | 0  0  0  1 |
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

	Matrix4x4 Matrix4x4::make_view_matrix(const vector::Vector3d& eye, const vector::Vector3d& target, const vector::Vector3d& up)
	{
		// Compute the forward (z), right (x), and up (y) vectors
		vector::Vector3d z{ target - eye };
		vector::Vector3d x{ up.cross_product(target) };
		z.normalize();
		x.normalize();
		vector::Vector3d y{ z.cross_product(x) };

		// | x.x   x.y   x.z  -dot(x,eye) |
		// | y.x   y.y   y.z  -dot(y,eye) |
		// | z.x   z.y   z.z  -dot(z,eye) |
		// |   0     0     0            1 |
		return {
			{x.m_x, x.m_y, x.m_z, -(x.dot_product(eye))},
			{y.m_x, y.m_y, y.m_z, -(y.dot_product(eye))},
			{z.m_x, z.m_y, z.m_z, -(z.dot_product(eye))},
			{0, 0, 0, 1},
		};
	}

	double Matrix4x4::get_determinant() const
	{
		// Cofactor expansion along the first row
		double det = m_matrix[0][0] * get_submatrix_determinant(0, 0) -
			m_matrix[0][1] * get_submatrix_determinant(0, 1) +
			m_matrix[0][2] * get_submatrix_determinant(0, 2) -
			m_matrix[0][3] * get_submatrix_determinant(0, 3);
		return det;
	}

	Matrix4x4 Matrix4x4::get_inverse() const
	{
		double determinant = get_determinant();
		if (determinant == 0) {
			// The matrix is not invertible
			return Matrix4x4();  // Return an empty matrix or handle the error as needed
		}

		Matrix4x4 adjugate;

		// Calculate the adjugate matrix
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				adjugate.m_matrix[j][i] = get_cofactor(i, j); // Transpose of cofactor matrix
			}
		}

		// Divide each element of the adjugate matrix by the determinant
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				adjugate.m_matrix[i][j] /= determinant;
			}
		}

		return adjugate;
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m)
	{
		matrix::Matrix4x4 world{};
		// because we are in a loop, if we directly update the values, they change during the loop 
		// so we need to capture the transformation in a new matrix and then copy over those values
		for (std::size_t i{ 0 }; i < 4; i++)
		{
			for (std::size_t j{ 0 }; j < 4; j++)
			{
				world.m_matrix[i][j] = m.m_matrix[i][0] * this->m_matrix[0][j] + m.m_matrix[i][1] * this->m_matrix[1][j] + m.m_matrix[i][2] * this->m_matrix[2][j] + m.m_matrix[i][3] * this->m_matrix[3][j];
			}
		}
		for (std::size_t i{ 0 }; i < 4; i++)
		{
			for (std::size_t j{ 0 }; j < 4; j++)
			{
				this->m_matrix[i][j] = world.m_matrix[i][j];
			}
		}
		return *this;
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

	vector::Vector4d Matrix4x4::project(const vector::Vector4d& vec4d) const
	{
		vector::Vector4d projection{ mult_vec4d(vec4d) };

		// do perspective divide
		if (projection.m_w != 0.0)
		{
			projection.m_x /= projection.m_w;
			projection.m_y /= projection.m_w;
			projection.m_z /= projection.m_w;
		}
		return projection;
	}

	// | 1 0 0 |
	// | 0 1 0 |
	// | 0 0 1 |
	Matrix3x3::Matrix3x3()
		: m_matrix{
			{ 1.0, 0, 0},
			{ 0, 1.0, 0},
			{ 0, 0, 1.0}
		}
	{
	};
	Matrix3x3::Matrix3x3(const Matrix4x4& mat4)
		: m_matrix{
			mat4.m_matrix[0],
			mat4.m_matrix[1],
			mat4.m_matrix[2]
		}
	{
	};

	Matrix3x3 Matrix3x3::get_transpose() const
	{
		matrix::Matrix3x3 transpose{};

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				transpose.m_matrix[i][j] = m_matrix[j][i];
			}
		}

		return transpose;
	}

	Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& m)
	{
		matrix::Matrix3x3 world{};
		// because we are in a loop, if we directly update the values, they change during the loop 
		// so we need to capture the transformation in a new matrix and then copy over those values
		for (std::size_t i{ 0 }; i < 3; i++)
		{
			for (std::size_t j{ 0 }; j < 3; j++)
			{
				world.m_matrix[i][j] = m.m_matrix[i][0] * this->m_matrix[0][j] + m.m_matrix[i][1] * this->m_matrix[1][j] + m.m_matrix[i][2] * this->m_matrix[2][j];
			}
		}
		for (std::size_t i{ 0 }; i < 3; i++)
		{
			for (std::size_t j{ 0 }; j < 3; j++)
			{
				this->m_matrix[i][j] = world.m_matrix[i][j];
			}
		}
		return *this;
	}

	Matrix3x3& Matrix3x3::operator*=(const Matrix4x4& m)
	{
		matrix::Matrix3x3 world{};
		// because we are in a loop, if we directly update the values, they change during the loop 
		// so we need to capture the transformation in a new matrix and then copy over those values
		for (std::size_t i{ 0 }; i < 3; i++)
		{
			for (std::size_t j{ 0 }; j < 3; j++)
			{
				world.m_matrix[i][j] = m.m_matrix[i][0] * this->m_matrix[0][j] + m.m_matrix[i][1] * this->m_matrix[1][j] + m.m_matrix[i][2] * this->m_matrix[2][j];
			}
		}
		for (std::size_t i{ 0 }; i < 3; i++)
		{
			for (std::size_t j{ 0 }; j < 3; j++)
			{
				this->m_matrix[i][j] = world.m_matrix[i][j];
			}
		}
		return *this;
	}

	vector::Vector3d Matrix3x3::mult_vec3d(const vector::Vector3d& vec3d) const
	{
		return vector::Vector3d{
			vec3d.m_x * m_matrix[0][0] + vec3d.m_y * m_matrix[0][1] + vec3d.m_z * m_matrix[0][2],
			vec3d.m_x * m_matrix[1][0] + vec3d.m_y * m_matrix[1][1] + vec3d.m_z * m_matrix[1][2],
			vec3d.m_x * m_matrix[2][0] + vec3d.m_y * m_matrix[2][1] + vec3d.m_z * m_matrix[2][2]
		};
	}

	double Matrix3x3::get_determinant() const
	{
		//m00m11m22 + m01m12m20 + m02m10m21 - m02m11m20 - m01m10m22 - m00m12m21
		return (m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][2]) +
			(m_matrix[0][1] * m_matrix[1][2] * m_matrix[2][0]) +
			(m_matrix[0][2] * m_matrix[1][0] * m_matrix[2][1]) -
			(m_matrix[0][2] * m_matrix[1][1] * m_matrix[2][0]) -
			(m_matrix[0][1] * m_matrix[1][0] * m_matrix[2][2]) -
			(m_matrix[0][0] * m_matrix[1][2] * m_matrix[2][1]);
	}

	Matrix3x3 Matrix3x3::get_inverse() const
	{
		double determinant{ get_determinant() };
		Matrix3x3 inverse{};
		inverse.m_matrix[0][0] = (m_matrix[1][1] * m_matrix[2][2] - m_matrix[2][1] * m_matrix[1][2]) / determinant;
		inverse.m_matrix[0][1] = -(m_matrix[1][0] * m_matrix[2][2] - m_matrix[2][0] * m_matrix[1][2]) / determinant;
		inverse.m_matrix[0][2] = (m_matrix[1][0] * m_matrix[2][1] - m_matrix[2][0] * m_matrix[1][1]) / determinant;
		inverse.m_matrix[1][0] = -(m_matrix[0][1] * m_matrix[2][2] - m_matrix[2][1] * m_matrix[0][2]) / determinant;
		inverse.m_matrix[1][1] = (m_matrix[0][0] * m_matrix[2][2] - m_matrix[2][0] * m_matrix[0][2]) / determinant;
		inverse.m_matrix[1][2] = -(m_matrix[0][0] * m_matrix[2][1] - m_matrix[2][0] * m_matrix[0][1]) / determinant;
		inverse.m_matrix[2][0] = (m_matrix[0][1] * m_matrix[1][2] - m_matrix[1][1] * m_matrix[0][2]) / determinant;
		inverse.m_matrix[2][1] = -(m_matrix[0][0] * m_matrix[1][2] - m_matrix[1][0] * m_matrix[0][2]) / determinant;
		inverse.m_matrix[2][2] = (m_matrix[0][0] * m_matrix[1][1] - m_matrix[1][0] * m_matrix[0][1]) / determinant;
		return inverse;
	}

	// | 1 0 |
	// | 0 1 |
	Matrix2x2::Matrix2x2()
		: m_matrix{
			{ 1.0, 0},
			{ 0, 1.0}
		}
	{
	};

	Matrix2x2::Matrix2x2(const std::vector<double>& i1, const std::vector<double>& i2)
		: m_matrix{
			i1,
			i2
		}
	{
	};

	double Matrix2x2::get_determinant() const
	{
		//m00m11 - m01m10
		return (m_matrix[0][0] * m_matrix[0][1]) - (m_matrix[0][1] * m_matrix[1][0]);
	}
}