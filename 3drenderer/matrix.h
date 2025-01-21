#pragma once
#include <vector>
#include "vector4d.h"

namespace matrix
{
	class Matrix4x4
	{
	public:
		Matrix4x4();
		static Matrix4x4 make_scale_matrix(double scale_x, double scale_y, double scale_z);
		vector::Vector4d mult_vec4d(const vector::Vector4d& vec4d) const;

		std::vector<std::vector<double>> m_matrix;
	};
}