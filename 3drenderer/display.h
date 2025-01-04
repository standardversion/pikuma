#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <vector>
#include "sdlwrapper.h"
#include "vector2d.h"
#include "triangle.h"


//https://wiki.libsdl.org/SDL2/CategoryAPI

namespace display
{
	inline int FPS{ 30 };
	inline double FRAME_TARGET_TIME(1000 / FPS);
	class Display
	{
	public:
		virtual ~Display() = default;
		virtual void cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer, const SDLWrapper& sdl) const;
		virtual void clear_colour_buffer(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t colour) const;
		virtual void draw_grid(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t line_colour, const std::uint32_t bg_colour, const int grid_on) const;
		virtual void draw_line(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const std::uint32_t colour) const;
		virtual void draw_pixel(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x, int y, const std::uint32_t colour) const;
		virtual void draw_rect(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int start_x, int start_y, int width, int height, const std::uint32_t colour) const;
		virtual void draw_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::triangle_t& triangle, const std::uint32_t colour) const;
		virtual bool initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode, const SDLWrapper& sdl) const;
		virtual void render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, SDL_Renderer*& renderer, const SDLWrapper& sdl) const;
		virtual bool setup(SDL_Texture*& colour_buffer_texture, SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* displaymode, const SDLWrapper& sdl) const;
	};
}

