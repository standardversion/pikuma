#pragma once
#include <vector>
#include "vector4d.h"

namespace matrix
{
	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(const double fov, const double aspect, const double znear, const double zfar);
		static Matrix4x4 make_scale_matrix(const double scale_x, const double scale_y, const double scale_z);
		static Matrix4x4 make_translation_matrix(const double transalation_x, const double transalation_y, const double transalation_z);
		static Matrix4x4 make_rotation_matrix(const double angle, const char axis = 'x');
		Matrix4x4& operator*=(const Matrix4x4& m);
		
		virtual vector::Vector4d mult_vec4d(const vector::Vector4d& vec4d) const;
		virtual vector::Vector4d project(const vector::Vector4d& vec4d) const;

		std::vector<std::vector<double>> m_matrix;
	};
}