#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include "display.h"
#include "input.h"

void update()
{

}


int main(int argc, char* argv[])
{
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* colour_buffer_texture{ nullptr };
	SDL_DisplayMode display_mode;
	
	bool is_running{ display::setup(colour_buffer_texture, window, renderer, &display_mode) };
	std::uint32_t* colour_buffer{ new std::uint32_t[display_mode.w * display_mode.h]{} };
	while (is_running)
	{
		input::process(is_running);
		update();
		display::render(renderer, colour_buffer_texture, colour_buffer, &display_mode);
	}
	display::cleanup(window, renderer, colour_buffer);
	return 0;
}