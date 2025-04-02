#pragma once
#include <vector>
#include "vector3d.h"

namespace clip
{
	enum ClipPlane
	{
		LEFT_FRUSTRUM_PLANE,
		RIGHT_FRUSTRUM_PLANE,
		TOP_FRUSTRUM_PLANE,
		BOTTOM_FRUSTRUM_PLANE,
		NEAR_FRUSTRUM_PLANE,
		FAR_FRUSTRUM_PLANE,
		TOTAL_PLANES
	};

	struct plane_t
	{
		vector::Vector3d m_point;
		vector::Vector3d m_normal;
	};

	std::vector<plane_t> init_frustum_planes(const double fov_x, const double fov_y, const double z_near, const double z_far);
}
