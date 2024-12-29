#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include "display.h"
#include "input.h"
#include "sdlwrapper.h"

void update()
{

}


int main(int argc, char* argv[])
{
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* colour_buffer_texture{ nullptr };
	SDL_DisplayMode display_mode;
	SDLWrapper sdl{};
	display::Display display{};
	input::Input input{};
	bool is_running{ display.setup(colour_buffer_texture, window, renderer, &display_mode, sdl) };
	std::uint32_t* colour_buffer{ new std::uint32_t[display_mode.w * display_mode.h]{} };
	std::uint32_t bg_colour{ 0x00000000 };
	std::uint32_t line_colour{ 0xFFFFFFFF };
	std::uint32_t rect_colour{ 0xFFFFFF00 };
	SDL_Event event{};
	while (is_running)
	{
		input.process(is_running, event, sdl);
		update();
		display.render(
			renderer,
			colour_buffer_texture,
			colour_buffer,
			&display_mode,
			line_colour,
			bg_colour,
			10,
			0,
			0,
			10,
			10,
			rect_colour,
			sdl
		);
	}
	display.cleanup(window, renderer, colour_buffer, sdl);
	return 0;
}