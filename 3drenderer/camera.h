#pragma once
#include "vector3d.h"

namespace camera
{
	struct camera_t
	{
		vector::Vector3d m_position;
		vector::Vector3d m_direction;
	};
}
