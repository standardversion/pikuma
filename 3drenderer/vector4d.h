#pragma once
#include "vector3d.h"

namespace vector
{
	class Vector4d
	{
	public:
		Vector4d(const double x = 0.0, const double y = 0.0, const double z = 0.0, const double w = 1.0);
		Vector4d(const vector::Vector3d& vec3d);

		double m_x{ 0.0 };
		double m_y{ 0.0 };
		double m_z{ 0.0 };
		double m_w{ 0.0 };
	};
}