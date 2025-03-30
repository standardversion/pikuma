#pragma once
#include <vector>
#include "vector3d.h"
#include "vector4d.h"

namespace matrix
{
	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(const std::vector<double>& v1, const std::vector<double>& v2, const std::vector<double>& v3, const std::vector<double>& v4);
		Matrix4x4(const double fov, const double aspect, const double znear, const double zfar);
		double get_determinant() const;
		Matrix4x4 get_inverse() const;
		Matrix4x4 get_transpose() const;
		static Matrix4x4 make_scale_matrix(const double scale_x, const double scale_y, const double scale_z);
		static Matrix4x4 make_translation_matrix(const double transalation_x, const double transalation_y, const double transalation_z);
		static Matrix4x4 make_rotation_matrix(const double angle, const char axis = 'x');
		static Matrix4x4 make_view_matrix(const vector::Vector3d& eye, const vector::Vector3d& target, const vector::Vector3d& up);
		Matrix4x4& operator*=(const Matrix4x4& m);
		
		virtual vector::Vector4d mult_vec4d(const vector::Vector4d& vec4d) const;
		virtual vector::Vector4d project(const vector::Vector4d& vec4d) const;

		std::vector<std::vector<double>> m_matrix;
	private:
		double get_submatrix_determinant(int r, int c) const;
		double get_cofactor(int r, int c) const;
	};

	class Matrix3x3
	{
	public:
		Matrix3x3();
		Matrix3x3(const Matrix4x4& mat4);
		Matrix3x3 get_transpose() const;
		double get_determinant() const;
		Matrix3x3 get_inverse() const;
		Matrix3x3& operator*=(const Matrix3x3& m);
		Matrix3x3& operator*=(const Matrix4x4& m);

		virtual vector::Vector3d mult_vec3d(const vector::Vector3d& vec3d) const;

		std::vector<std::vector<double>> m_matrix;
	};

	class Matrix2x2
	{
	public:
		Matrix2x2();
		Matrix2x2(const std::vector<double>& i1, const std::vector<double>& i2);
		double get_determinant() const;

		std::vector<std::vector<double>> m_matrix;
	};
}