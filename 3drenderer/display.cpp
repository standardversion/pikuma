#include "display.h"

namespace display {
	bool initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode, SDLWrapper& sdl)
	{
		// Initialize the SDL library
		// SDL_INIT_EVERYTHING flag initializes audio, video, controller etc subsystems
		if (sdl.SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cout << "Error initializing SLD.\n";
			return false;
		}
		// Get the current display mode so we can figure out current display dimensions
		if (sdl.SDL_GetCurrentDisplayMode(0, display_mode) != 0)
		{
			std::cout << "Error getting SLD Display mode.\n";
			return false;
		}
		// Create the SDL Window
		window = sdl.SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			display_mode->w,
			display_mode->h,
			SDL_WINDOW_BORDERLESS
		);
		if (!window)
		{
			std::cout << "Error creating SDL window.\n";
			return false;
		}
		// Create SDL Renderer, a 2d rendering context for a window
		// -1 to initialize the first driver supporting the requested flags.
		// 0 is no flags requested
		renderer = sdl.SDL_CreateRenderer(window, -1, 0);

		if (!renderer)
		{
			std::cout << "Error creating SDL renderer.\n";
			return false;
		}

		sdl.SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		return true;
	}

	void draw_grid(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, std::uint32_t line_colour, std::uint32_t bg_colour)
	{
		for (int y{ 0 }; y < display_mode->h; y++)
		{
			bool fill_y = y == 0 ? false : y % 10 ? false : true;
			for (int x{ 0 }; x < display_mode->w; x++)
			{
				bool fill_x = x == 0 ? false : x % 10 ? false : true;
				std::uint32_t colour = fill_x || fill_y ? line_colour : bg_colour;
				colour_buffer[(display_mode->w * y) + x] = colour;
			}
		}
	}

	void draw_rect(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, int start_x, int start_y, int width, int height, std::uint32_t colour)
	{
		for (int y{ start_y }; y < height; y++)
		{
			for (int x{ start_x }; x < width; x++)
			{
				colour_buffer[(display_mode->w * y) + x] = colour;
			}
		}
	}

	void clear_colour_buffer(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, std::uint32_t colour)
	{
		// our 2d pixels (x * y) are laid out in a 1d array
		// so for example, if we're dealing with 1920x1080 the first 1920 indices for the array
		// will contain row 1 of the pixels on the screen 1921 to 3840 will contain row 2 ...
		// so we start with the height ie row one then loop across  with width horizontally to fill that row
		// with colour values
		// to access any specific pixel we can get the index using [(width * row) + column]
		for (int y{ 0 }; y < display_mode->h; y++)
		{
			for (int x{ 0 }; x < display_mode->w; x++)
			{
				colour_buffer[(display_mode->w * y) + x] = colour;
			}
		}
	}

	void render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, SDL_Renderer*& renderer, SDLWrapper& sdl)
	{
		// update the texture with the contents of the colour buffer
		sdl.SDL_UpdateTexture(
			colour_buffer_texture,
			NULL,
			colour_buffer,
			static_cast<int>(display_mode->w * sizeof(std::uint32_t))
		);
		// copy the texture onto the renderer, null, null copies entire texture
		sdl.SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL);
	}

	void render(SDL_Renderer*& renderer, SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, SDLWrapper& sdl)
	{
		// set the renderer colour to black
		sdl.SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// clear the renderer
		sdl.SDL_RenderClear(renderer);

		draw_grid(colour_buffer, display_mode, 0xFFFFFFFF, 0x00000000);

		draw_rect(colour_buffer, display_mode, 500, 100, 1000, 1000, 0xFFFFFF00);

		// render the colour buffer
		render_colour_buffer(colour_buffer_texture, colour_buffer, display_mode, renderer, sdl);
		// fill the colour buffer with a colour value
		clear_colour_buffer(colour_buffer, display_mode, 0x00000000);

		// Update the screen with any rendering performed since the previous call.
		sdl.SDL_RenderPresent(renderer);
	}

	void cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer, SDLWrapper& sdl)
	{
		// SDL cleanup
		sdl.SDL_DestroyRenderer(renderer);
		sdl.SDL_DestroyWindow(window);
		sdl.SDL_Quit();
		// set pointers to nullptr
		renderer = nullptr;
		window = nullptr;
		// deallocate the memory
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	bool setup(SDL_Texture*& colour_buffer_texture, SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* displaymode, SDLWrapper& sdl)
	{
		// first create the window and rendering context
		bool initiatized = display::initialize_window(window, renderer, displaymode, sdl);
		// Create a texture for a rendering context. ie to display the colour buffer
		colour_buffer_texture = sdl.SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			displaymode->w,
			displaymode->h
		);

		return initiatized;
	}
}