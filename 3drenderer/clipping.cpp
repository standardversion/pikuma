#include <math.h>
#include "clipping.h"

namespace clip
{
	///////////////////////////////////////////////////////////////////////////////
	// Frustum planes are defined by a point and a normal vector
	///////////////////////////////////////////////////////////////////////////////
	// Near plane   :  P=(0, 0, znear), N=(0, 0,  1)
	// Far plane    :  P=(0, 0, zfar),  N=(0, 0, -1)
	// Top plane    :  P=(0, 0, 0),     N=(0, -cos(fov/2), sin(fov/2))
	// Bottom plane :  P=(0, 0, 0),     N=(0, cos(fov/2), sin(fov/2))
	// Left plane   :  P=(0, 0, 0),     N=(cos(fov/2), 0, sin(fov/2))
	// Right plane  :  P=(0, 0, 0),     N=(-cos(fov/2), 0, sin(fov/2))
	///////////////////////////////////////////////////////////////////////////////
	//
	//           /|\
	//         /  | | 
	//       /\   | |
	//     /      | |
	//  P*|-->  <-|*|   ----> +z-axis
	//     \      | |
	//       \/   | |
	//         \  | | 
	//           \|/
	//
	///////////////////////////////////////////////////////////////////////////////

	std::vector<plane_t> init_frustum_planes(const double fov, const double z_near, const double z_far)
	{
		vector::Vector3d origin{ 0.0, 0.0, 0.0 };
		std::vector<plane_t> planes(6);
		double cos_half_fov{ cos(fov / 2) };
		double sin_half_fov{ cos(fov / 2) };

		plane_t left_plane{ .m_point{ origin }, .m_normal{ cos_half_fov, 0, sin_half_fov } };
		plane_t right_plane{ .m_point{ origin }, .m_normal{ -cos_half_fov, 0, sin_half_fov } };
		plane_t top_plane{ .m_point{ origin }, .m_normal{ 0, -cos_half_fov, sin_half_fov } };
		plane_t bottom_plane{ .m_point{ origin }, .m_normal{ 0, cos_half_fov, sin_half_fov } };
		plane_t near_plane{ .m_point{ 0, 0, z_near }, .m_normal{ 0, 0, 1 } };
		plane_t far_plane{ .m_point{ 0, 0, z_far }, .m_normal{ 0, 0, -1 } };

		return planes;
	}
}