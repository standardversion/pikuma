#pragma once
#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include "sdlwrapper.h"

//https://wiki.libsdl.org/SDL2/CategoryAPI

namespace display {
	bool initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode, SDLWrapper& sdl);
	void draw_grid(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, std::uint32_t line_colour, std::uint32_t bg_colour, int grid_on);
	void draw_rect(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, int start_x, int start_y, int width, int height, std::uint32_t colour);
	void clear_colour_buffer(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, std::uint32_t colour);
	void render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, SDL_Renderer*& renderer, SDLWrapper& sdl);
	void render(SDL_Renderer*& renderer, SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, SDLWrapper& sdl);
	void cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer, SDLWrapper& sdl);
	bool setup(SDL_Texture*& colour_buffer_texture, SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* displaymode, SDLWrapper& sdl);
}

