#pragma once
#include <SDL.h>
#include <cstdint>

namespace display
{
	static SDL_Window* window{ nullptr };
	static SDL_Renderer* renderer{ nullptr };
	static SDL_Texture* colour_buffer_texture{ nullptr };
	static SDL_DisplayMode display_mode;
	static std::uint32_t* colour_buffer{ nullptr };
	static int low_rez_factor{ 5 };

	bool setup();
	void render_colour_buffer();
	void clear_colour_buffer();
	void draw_pixel(int x, int y, const std::uint32_t colour);
}

