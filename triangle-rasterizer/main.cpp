#include "display.h"
#include "input.h"

int main(int argc, char* argv[])
{
	bool is_running{ display::setup() };
	while (is_running)
	{
		input::process(is_running);
		display::draw_pixel(1500, 500, 0xFFFFFF00);
		display::render_colour_buffer();
		display::clear_colour_buffer();
	}
	return 0;
}
