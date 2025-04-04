#include <vector>
#include <algorithm>
#include <cstdint>
#include "display.h"
#include "input.h"
#include "vector2d.h"
#include "colour.h"


bool is_top_or_left(vec::v2d edge)
{
	bool is_top_edge{ edge.y == 0 && edge.x > 0 }; 
	bool is_left_edge{ edge.y < 0 };
	return is_top_edge || is_left_edge;
}


void fill_triangle(const vec::v2d& a, const vec::v2d& b, const vec::v2d& c, const std::vector<col::colour_t>& colours, bool top_left_rasterization)
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

	double area{ ab.cross_product(bc) };

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

			double w0{ ab.cross_product(ap) };
			double w1{ bc.cross_product(bp) };
			double w2{ ca.cross_product(cp) };

			if (top_left_rasterization)
			{
				w0 += bias0;
				w1 += bias1;
				w2 += bias2;
			}

			bool is_inside{ false };
			is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

			if (is_inside)
			{
				double alpha{ w0 / area };
				double beta{ w1 / area };
				double gamma{ w2 / area };

				int colour_a = 0xFF;
				int colour_r = (alpha)*colours[0].r + (beta)*colours[1].r + (gamma)*colours[2].r;
				int colour_g = (alpha)*colours[0].g + (beta)*colours[1].g + (gamma)*colours[2].g;
				int colour_b = (alpha)*colours[0].b + (beta)*colours[1].b + (gamma)*colours[2].b;

				uint32_t interp_colour = 0x00000000;
				interp_colour = (interp_colour | colour_a) << 8;
				interp_colour = (interp_colour | colour_b) << 8;
				interp_colour = (interp_colour | colour_g) << 8;
				interp_colour = (interp_colour | colour_r);

				display::draw_pixel(x, y, interp_colour);
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
	std::vector<col::colour_t> colours{
		{ 0xFF, 0x00, 0x00 },
		{ 0x00, 0xFF, 0x00 },
		{ 0x00, 0x00, 0xFF }
	};
	bool top_left_rasterization{ true };

	while (is_running)
	{
		input::process(is_running, top_left_rasterization);
		fill_triangle(vertices[0], vertices[1], vertices[2], colours, top_left_rasterization);
		fill_triangle(vertices[3], vertices[2], vertices[1], colours, top_left_rasterization);
		fill_triangle(vertices[4], vertices[1], vertices[0], colours, top_left_rasterization);
		display::render_colour_buffer();
		display::clear_colour_buffer();
	}
	return 0;
}
