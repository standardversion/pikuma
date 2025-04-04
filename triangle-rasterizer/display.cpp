#include <iostream>
#include "display.h"

namespace display
{
	bool setup()
	{
		bool initialized{ false };
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cout << "Error initializing SLD.\n";
			return initialized;
		}
		// Get the current display mode so we can figure out current display dimensions
		if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0)
		{
			std::cout << "Error getting SLD Display mode.\n";
			return initialized;
		}
		// Create the SDL Window
		window = SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			display_mode.w/low_rez_factor,
			display_mode.h/low_rez_factor,
			SDL_WINDOW_SHOWN
		);
		if (!window)
		{
			std::cout << "Error creating SDL window.\n";
			return initialized;
		}
		// Create SDL Renderer, a 2d rendering context for a window
		// -1 to initialize the first driver supporting the requested flags.
		// 0 is no flags requested
		renderer = SDL_CreateRenderer(window, -1, 0);

		if (!renderer)
		{
			std::cout << "Error creating SDL renderer.\n";
			return initialized;
		}

		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		// Create a texture for a rendering context. ie to display the colour buffer
		colour_buffer_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			display_mode.w/low_rez_factor,
			display_mode.h/low_rez_factor
		);
		colour_buffer = new std::uint32_t[display_mode.w/low_rez_factor * display_mode.h/low_rez_factor]{};
		initialized = true;
		return initialized;
	}

	void render_colour_buffer()
	{
		SDL_UpdateTexture(
			colour_buffer_texture,
			NULL,
			colour_buffer,
			static_cast<int>(display_mode.w/low_rez_factor * sizeof(std::uint32_t))
		);
		// copy the texture onto the renderer, null, null copies entire texture
		SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	void clear_colour_buffer()
	{
		for (int y{ 0 }; y < display_mode.h/low_rez_factor; y++)
		{
			for (int x{ 0 }; x < display_mode.w/low_rez_factor; x++)
			{
				colour_buffer[(display_mode.w/low_rez_factor * y) + x] = 0x00000000;
			}
		}
	}

	void draw_pixel(int x, int y, const std::uint32_t colour)
	{
		if (x >= 0 && x < display_mode.w/low_rez_factor && y >= 0 && y < display_mode.h/low_rez_factor)
		{
			colour_buffer[(display_mode.w/low_rez_factor * y) + x] = colour;
		}
	}

	vec::v2d get_screen_dimension()
	{
		return {
			double(display_mode.h / low_rez_factor),
			double(display_mode.h / low_rez_factor)
		};
	}
}