#pragma once
#include "vector3d.h"

namespace shading
{
	class Light
	{
	public:
		Light();

		vector::Vector3d m_direction{ 0.0, 0.0, 1.0 };
	};
}
