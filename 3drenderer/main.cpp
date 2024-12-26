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
	SDLWrapper sdl(
		&SDL_CreateRenderer,
		&SDL_CreateTexture,
		&SDL_CreateWindow,
		&SDL_DestroyRenderer,
		&SDL_DestroyWindow,
		&SDL_GetCurrentDisplayMode,
		&SDL_Init,
		&SDL_PollEvent,
		&SDL_Quit,
		&SDL_RenderClear,
		&SDL_RenderCopy,
		&SDL_RenderPresent,
		&SDL_SetRenderDrawColor,
		&SDL_SetWindowFullscreen,
		&SDL_UpdateTexture
	);
	
	bool is_running{ display::setup(colour_buffer_texture, window, renderer, &display_mode, sdl) };
	std::uint32_t* colour_buffer{ new std::uint32_t[display_mode.w * display_mode.h]{} };
	while (is_running)
	{
		input::process(is_running, sdl);
		update();
		display::render(renderer, colour_buffer_texture, colour_buffer, &display_mode, sdl);
	}
	display::cleanup(window, renderer, colour_buffer, sdl);
	return 0;
}