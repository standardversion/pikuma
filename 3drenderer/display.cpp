#include "display.h"
#include "utils.h"

namespace display
{
	void Display::cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer, const SDLWrapper& sdl) const
	{
		sdl.SDL_DestroyRenderer(renderer);
		sdl.SDL_DestroyWindow(window);
		sdl.SDL_Quit();
		renderer = nullptr;
		window = nullptr;
		// deallocate the memory
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	void Display::clear_colour_buffer(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t colour) const
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

	void Display::draw_grid(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const std::uint32_t line_colour, const std::uint32_t bg_colour, const int grid_on) const
	{
		for (int y{ 0 }; y < display_mode->h; y++)
		{
			bool fill_y = (y + 1) % grid_on ? false : true;
			for (int x{ 0 }; x < display_mode->w; x++)
			{
				bool fill_x = (x + 1) % grid_on ? false : true;
				std::uint32_t colour = fill_x || fill_y ? line_colour : bg_colour;
				colour_buffer[(display_mode->w * y) + x] = colour;
			}
		}
	}

	void Display::draw_line(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const std::uint32_t colour) const
	{
		int delta_x{ x1 - x0 };
		int delta_y{ y1 - y0 };

		int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);
		// Find out how much we should increment in x and y in each iteration
		double x_increment = delta_x / static_cast<double>(side_length);
		double y_increment = delta_y / static_cast<double>(side_length);

		double current_x{ static_cast<double>(x0) };
		double current_y{ static_cast<double>(y0) };
		for (int i{ 0 }; i <= side_length; i++)
		{
			draw_pixel(colour_buffer, display_mode, round(current_x), round(current_y), colour);
			current_x += x_increment;
			current_y += y_increment;
		}
	}

	void Display::draw_pixel(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x, int y, const std::uint32_t colour) const
	{
		if (x >= 0 && x < display_mode->w && y>= 0 &&  y < display_mode->h)
		{
			colour_buffer[(display_mode->w * y) + x] = colour;
		}
		
	}

	void Display::draw_rect(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int start_x, int start_y, int width, int height, const std::uint32_t colour) const
	{
		for (int i{ 0 }; i < width; i++)
		{
			for (int j{ 0 }; j < height; j++)
			{
				draw_pixel(colour_buffer, display_mode, start_x + i, start_y + j, colour);
			}
		}
	}

	void Display::draw_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<double>& triangle, const std::uint32_t colour) const
	{
		draw_line(
			colour_buffer,
			display_mode,
			triangle.m_points[0].m_x,
			triangle.m_points[0].m_y,
			triangle.m_points[1].m_x,
			triangle.m_points[1].m_y,
			colour
		);
		draw_line(
			colour_buffer,
			display_mode,
			triangle.m_points[0].m_x,
			triangle.m_points[0].m_y,
			triangle.m_points[2].m_x,
			triangle.m_points[2].m_y,
			colour
		);
		draw_line(
			colour_buffer,
			display_mode,
			triangle.m_points[1].m_x,
			triangle.m_points[1].m_y,
			triangle.m_points[2].m_x,
			triangle.m_points[2].m_y,
			colour
		);
	}

	void Display::fill_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, geo::Triangle<int>& triangle, const std::uint32_t colour) const
	{
		// sort the triangle by the y component
		triangle.sort();

		if (triangle.m_points[1].m_y == triangle.m_points[2].m_y)
		{
			fill_flat_bottom_triangle(colour_buffer, display_mode, triangle, colour);
		}
		else if (triangle.m_points[0].m_y == triangle.m_points[1].m_y)
		{
			fill_flat_top_triangle(colour_buffer, display_mode, triangle, colour);
		}
		else {
			vector::Vector2d<int> midpoint{ triangle.get_midpoint() };
			geo::Triangle<int> flat_bottom_triangle{ triangle.m_points[0], triangle.m_points[1], midpoint };
			geo::Triangle<int> flat_top_triangle{ triangle.m_points[1], midpoint, triangle.m_points[2] };
			fill_flat_bottom_triangle(colour_buffer, display_mode, flat_bottom_triangle, colour);
			fill_flat_top_triangle(colour_buffer, display_mode, flat_top_triangle, colour);
		}
	}

	void Display::fill_flat_bottom_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour) const
	{
		double x_start_slope{ triangle.get_inverse_slope(1, 0) };
		double x_end_slope{ triangle.get_inverse_slope(2, 0) };
		double x_start{ static_cast<double>(triangle.m_points[0].m_x) };
		double x_end{ static_cast<double>(triangle.m_points[0].m_x) };
		for (int i{ triangle.m_points[0].m_y }; i <= triangle.m_points[1].m_y; i++)
		{
			draw_line(
				colour_buffer,
				display_mode,
				x_start,
				i,
				x_end,
				i,
				colour
			);
			x_start += x_start_slope;
			x_end += x_end_slope;
		}
	}

	void Display::fill_flat_top_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour) const
	{
		double x_start_slope{ triangle.get_inverse_slope(2, 0) };
		double x_end_slope{ triangle.get_inverse_slope(2, 1) };
		double x_start{ static_cast<double>(triangle.m_points[2].m_x) };
		double x_end{ static_cast<double>(triangle.m_points[2].m_x) };
		for (int i{ triangle.m_points[2].m_y }; i >= triangle.m_points[0].m_y; i--)
		{
			draw_line(
				colour_buffer,
				display_mode,
				x_start,
				i,
				x_end,
				i,
				colour
			);
			x_start -= x_start_slope;
			x_end -= x_end_slope;
		}
	}

	bool Display::initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode, const SDLWrapper& sdl) const
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

	void Display::render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, SDL_Renderer*& renderer, const SDLWrapper& sdl) const
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

	bool Display::setup(SDL_Texture*& colour_buffer_texture, SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* displaymode, const SDLWrapper& sdl) const
	{
		// first create the window and rendering context
		bool initialized = initialize_window(window, renderer, displaymode, sdl);
		if (initialized) {
			// Create a texture for a rendering context. ie to display the colour buffer
			colour_buffer_texture = sdl.SDL_CreateTexture(
				renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				displaymode->w,
				displaymode->h
			);
		}
		return initialized;
	}
}