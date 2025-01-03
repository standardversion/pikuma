#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include "sdlwrapper.h"
#include "vector2d.h"


//https://wiki.libsdl.org/SDL2/CategoryAPI

namespace display
{
	inline int FPS{ 30 };
	inline double FRAME_TARGET_TIME(1000 / FPS);
	class Display
	{
	public:
		virtual ~Display() = default;
		virtual void cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer, SDLWrapper& sdl);
		virtual void clear_colour_buffer(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, std::uint32_t colour);
		virtual void draw_grid(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, std::uint32_t line_colour, std::uint32_t bg_colour, int grid_on);
		virtual void draw_pixel(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, int x, int y, std::uint32_t colour);
		virtual void draw_rect(std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, int start_x, int start_y, int width, int height, std::uint32_t colour);
		virtual bool initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode, SDLWrapper& sdl);
		virtual void render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, SDL_DisplayMode* display_mode, SDL_Renderer*& renderer, SDLWrapper& sdl);
		virtual bool setup(SDL_Texture*& colour_buffer_texture, SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* displaymode, SDLWrapper& sdl);
	};
}

