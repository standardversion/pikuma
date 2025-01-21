#include "vector4d.h"
#include "vector3d.h" // Full definition needed here since vector4d.h does not include vector3d.h to resolve circular dep

namespace vector
{
	Vector4d::Vector4d(const double x, const double y, const double z, const double w)
		: m_x{ x }, m_y{ y }, m_z{ z }, m_w{ w }
	{
	};

	Vector4d::Vector4d(const Vector3d& vec3d)
		: m_x{ vec3d.m_x }, m_y{ vec3d.m_y }, m_z{ vec3d.m_z }, m_w{ 1.0 }
	{
	};
}