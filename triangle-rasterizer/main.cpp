#include <vector>
#include <algorithm>
#include <cstdint>
#include "display.h"
#include "input.h"
#include "vector2d.h"


bool is_top_or_left(vec::v2d edge)
{
	bool is_top_edge{ edge.y == 0 && edge.x > 0 }; 
	bool is_left_edge{ edge.y < 0 };
	return is_top_edge || is_left_edge;
}


void fill_triangle(const vec::v2d& a, const vec::v2d& b, const vec::v2d& c, bool top_left_rasterization, const std::uint32_t colour)
{
	/*   A   */
	/*  / \  */
	/* C---B */
	
	int x_min{ (int)std::min(std::min(a.x, b.x), c.x) };
	int y_min{ (int)std::min(std::min(a.y, b.y), c.y) };
	int x_max{ (int)std::max(std::max(a.x, b.x), c.x) };
	int y_max{ (int)std::max(std::max(a.y, b.y), c.y) };

	vec::v2d ab{ b - a };
	vec::v2d bc{ c - b };
	vec::v2d ca{ a - c };

	// Fill convention (top-left rasterization rule)
	int bias0{ is_top_or_left(ab) ? 0 : -1 };
	int bias1{ is_top_or_left(bc) ? 0 : -1 };
	int bias2{ is_top_or_left(ca) ? 0 : -1 };


	for (int y{ y_min }; y <= y_max; y++)
	{
		for (int x{ x_min }; x <= x_max; x++)
		{
			vec::v2d pixel{ x, y };
			vec::v2d ap{ pixel - a };
			vec::v2d bp{ pixel - b };
			vec::v2d cp{ pixel - c };

			double ab_cross_ap{ ab.cross_product(ap) };
			double bc_cross_bp{ bc.cross_product(bp) };
			double ca_cross_cp{ ca.cross_product(cp) };

			if (top_left_rasterization)
			{
				ab_cross_ap += bias0;
				bc_cross_bp += bias1;
				ca_cross_cp += bias2;
			}

			bool is_inside{ false };
			is_inside = ab_cross_ap >= 0 && bc_cross_bp >= 0 && ca_cross_cp >= 0;

			if (is_inside)
			{
				display::draw_pixel(x, y, colour);
			}
		}
	}
}


int main(int argc, char* argv[])
{
	bool is_running{ display::setup() };
	std::vector<vec::v2d> vertices{
		{ 140, 140 },
		{ 180, 140 },
		{ 140, 180 },
		{ 190, 190 },
		{ 175, 120 }
	};
	bool top_left_rasterization{ true };

	while (is_running)
	{
		input::process(is_running, top_left_rasterization);
		fill_triangle(vertices[0], vertices[1], vertices[2], top_left_rasterization, 0xFFFFFF00);
		fill_triangle(vertices[3], vertices[2], vertices[1], top_left_rasterization, 0xFFA74DE3);
		fill_triangle(vertices[4], vertices[1], vertices[0], top_left_rasterization, 0xFF0390FC);
		display::render_colour_buffer();
		display::clear_colour_buffer();
	}
	return 0;
}
