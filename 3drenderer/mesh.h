#pragma once
#include <vector>
#include "vector3d.h"
#include "triangle.h"

namespace mesh
{
	inline std::vector<vector::Vector3d> mesh_vertices{
		{ -1, -1, -1 }, //1
		{ -1, 1, -1 }, //2
		{ 1, 1, -1 }, //3
		{ 1, -1, -1 }, //4
		{ 1, 1, 1 }, //5
		{ 1, -1, 1 }, //6
		{ -1, 1, 1 }, //7
		{ -1, -1, 1 } //8
	};

	inline std::vector<tri::face_t> mesh_faces{
		//front
		{ 1, 2, 3 },
		{ 1, 3, 4 },
		//right
		{ 4, 3, 5 },
		{ 4, 5, 6 },
		//back
		{ 6, 5, 7 },
		{ 6, 7, 8 },
		//left
		{ 8, 7, 2 },
		{ 8, 2, 1 },
		//top
		{ 2, 7, 5 },
		{ 2, 5, 3 },
		//bottom
		{ 6, 8, 1 },
		{ 6, 1, 4 }
	};
}