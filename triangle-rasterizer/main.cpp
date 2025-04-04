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

	// Compute the constant delta_s that will be used for the horizontal and vertical steps
	double delta_w0_col{ a.y - b.y };
	double delta_w1_col{ b.y - c.y };
	double delta_w2_col{ c.y - a.y };

	double delta_w0_row{ b.x - a.x };
	double delta_w1_row{ c.x - b.x };
	double delta_w2_row{ a.x - c.x };

	vec::v2d ab{ b - a };
	vec::v2d bc{ c - b };
	vec::v2d ca{ a - c };

	double area{ ab.cross_product(bc) };

	// Fill convention (top-left rasterization rule)
	int bias0{ is_top_or_left(ab) ? 0 : -1 };
	int bias1{ is_top_or_left(bc) ? 0 : -1 };
	int bias2{ is_top_or_left(ca) ? 0 : -1 };

	vec::v2d p0{ x_min, y_min };
	vec::v2d ap0{ p0 - a };
	vec::v2d bp0{ p0 - b };
	vec::v2d cp0{ p0 - c };
	double w0_row{ ab.cross_product(ap0) };
	double w1_row{ bc.cross_product(bp0) };
	double w2_row{ ca.cross_product(cp0) };

	if (top_left_rasterization)
	{
		w0_row += bias0;
		w1_row += bias1;
		w2_row += bias2;
	}

	for (int y{ y_min }; y <= y_max; y++)
	{
		double w0{ w0_row };
		double w1{ w1_row };
		double w2{ w2_row };
		for (int x{ x_min }; x <= x_max; x++)
		{
			bool is_inside{ w0 >= 0 && w1 >= 0 && w2 >= 0 };
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
			w0 += delta_w0_col;
			w1 += delta_w1_col;
			w2 += delta_w2_col;
		}
		w0_row += delta_w0_row;
		w1_row += delta_w1_row;
		w2_row += delta_w2_row;
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
