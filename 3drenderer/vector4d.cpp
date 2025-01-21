#include "vector4d.h"

namespace vector
{
	Vector4d::Vector4d(const double x = 0.0, const double y = 0.0, const double z = 0.0, const double w = 1.0)
		: m_x{ x }, m_y{ y }, m_z{ z }, m_w{ w }
	{
	};

	Vector4d::Vector4d(const vector::Vector3d& vec3d)
		: m_x{ vec3d.m_x }, m_y{ vec3d.m_y }, m_z{ vec3d.m_z }, m_w{ 1.0 }
	{
	};

	
}