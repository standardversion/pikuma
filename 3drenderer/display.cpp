#include "display.h"
#include "utils.h"

namespace display
{
	void activate_render_mode(const int render_mode, bool& render_wireframe, bool& render_vertex, bool& render_shaded, bool& render_face_center, bool& render_normals)
	{
		switch (render_mode)
		{
		case(display::RenderModes::wireframe):
			render_wireframe = true;
			render_vertex = false;
			render_shaded = false;
			render_face_center = false;
			render_normals = false;
			break;
		case(display::RenderModes::wireframe_vertex):
			render_wireframe = true;
			render_vertex = true;
			render_shaded = false;
			render_face_center = false;
			render_normals = false;
			break;
		case(display::RenderModes::wireframe_vertex_face_center):
			render_wireframe = true;
			render_vertex = true;
			render_shaded = false;
			render_face_center = true;
			render_normals = false;
			break;
		case(display::RenderModes::wireframe_vertex_face_center_normals):
			render_wireframe = true;
			render_vertex = true;
			render_shaded = false;
			render_face_center = true;
			render_normals = true;
			break;
		case(display::RenderModes::shaded):
			render_wireframe = false;
			render_vertex = false;
			render_shaded = true;
			render_face_center = false;
			render_normals = false;
			break;
		case(display::RenderModes::shaded_wireframe):
			render_wireframe = true;
			render_vertex = false;
			render_shaded = true;
			render_face_center = false;
			render_normals = false;
			break;
		case(display::RenderModes::shaded_wireframe_vertex):
			render_wireframe = true;
			render_vertex = true;
			render_shaded = true;
			render_face_center = false;
			render_normals = false;
			break;
		case(display::RenderModes::shaded_wireframe_vertex_face_center):
			render_wireframe = true;
			render_vertex = true;
			render_shaded = true;
			render_face_center = true;
			render_normals = false;
			break;
		case(display::RenderModes::shaded_wireframe_vertex_face_center_normals):
			render_wireframe = true;
			render_vertex = true;
			render_shaded = true;
			render_face_center = true;
			render_normals = true;
			break;
		default:
			break;
		}
	}

	std::uint32_t apply_light_intensity(const std::uint32_t colour, double percentage_factor)
	{
		if (percentage_factor < 0) percentage_factor = 0;
		if (percentage_factor > 1) percentage_factor = 1;
		const std::uint32_t a = (colour & 0xFF000000);
		const std::uint32_t r = (colour & 0x00FF0000) * percentage_factor;
		const std::uint32_t g = (colour & 0x0000FF00) * percentage_factor;
		const std::uint32_t b = (colour & 0x000000FF) * percentage_factor;

		return std::uint32_t{ a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF) };
	}

	void cleanup(std::uint32_t*& colour_buffer, double*& z_buffer)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		renderer = nullptr;
		window = nullptr;
		// deallocate the memory
		delete[] colour_buffer;
		colour_buffer = nullptr;
		delete[] z_buffer;
		z_buffer = nullptr;
	}

	void clear_colour_buffer(std::uint32_t*& colour_buffer, const std::uint32_t colour)
	{
		// our 2d pixels (x * y) are laid out in a 1d array
		// so for example, if we're dealing with 1920x1080 the first 1920 indices for the array
		// will contain row 1 of the pixels on the screen 1921 to 3840 will contain row 2 ...
		// so we start with the height ie row one then loop across  with width horizontally to fill that row
		// with colour values
		// to access any specific pixel we can get the index using [(width * row) + column]
		for (int y{ 0 }; y < display_mode.h; y++)
		{
			for (int x{ 0 }; x < display_mode.w; x++)
			{
				colour_buffer[(display_mode.w * y) + x] = colour;
			}
		}
	}

	void clear_z_buffer(double*& z_buffer)
	{
		// our 2d pixels (x * y) are laid out in a 1d array
		// so for example, if we're dealing with 1920x1080 the first 1920 indices for the array
		// will contain row 1 of the pixels on the screen 1921 to 3840 will contain row 2 ...
		// so we start with the height ie row one then loop across  with width horizontally to fill that row
		// with colour values
		// to access any specific pixel we can get the index using [(width * row) + column]
		for (int y{ 0 }; y < display_mode.h; y++)
		{
			for (int x{ 0 }; x < display_mode.w; x++)
			{
				z_buffer[(display_mode.w * y) + x] = 1.0;
			}
		}
	}

	void draw_grid(std::uint32_t*& colour_buffer, const std::uint32_t line_colour, const std::uint32_t bg_colour, const int grid_on)
	{
		for (int y{ 0 }; y < display_mode.h; y++)
		{
			bool fill_y = (y + 1) % grid_on ? false : true;
			for (int x{ 0 }; x < display_mode.w; x++)
			{
				bool fill_x = (x + 1) % grid_on ? false : true;
				std::uint32_t colour = fill_x || fill_y ? line_colour : bg_colour;
				colour_buffer[(display_mode.w * y) + x] = colour;
			}
		}
	}

	// draw line for flat shading
	void draw_line(std::uint32_t*& colour_buffer, int x0, int y0, int x1, int y1, const std::uint32_t colour)
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
			draw_pixel(colour_buffer, round(current_x), round(current_y), colour);
			current_x += x_increment;
			current_y += y_increment;
		}
	}

	void draw_pixel(std::uint32_t*& colour_buffer, int x, int y, const std::uint32_t colour)
	{
		if (x >= 0 && x < display_mode.w && y>= 0 &&  y < display_mode.h)
		{
			colour_buffer[(display_mode.w * y) + x] = colour;
		}
		
	}

	void draw_rect(std::uint32_t*& colour_buffer, int start_x, int start_y, int width, int height, const std::uint32_t colour)
	{
		for (int i{ 0 }; i < width; i++)
		{
			for (int j{ 0 }; j < height; j++)
			{
				draw_pixel(colour_buffer, start_x + i, start_y + j, colour);
			}
		}
	}

	std::uint32_t get_pixel_colour(const SDL_Surface* surface, const int x, const int y)
	{
		/*surface->pixels is a pointer to the raw pixel data of the SDL surface.

		surface->pitch is the number of bytes in a row of pixels(which can be larger than the width of the surface, due to padding).

		surface->format->BytesPerPixel is the number of bytes per pixel(typically 3 for 24 - bit images, 4 for 32 - bit images, etc.).

		x* surface->format->BytesPerPixel moves the pointer x pixels to the right.

		y* surface->pitch moves the pointer y rows down.

		The resulting pointer, pixel, points to the start of the pixel at the(x, y) coordinates in the surface's memory.*/

		std::uint8_t* pixel{ (std::uint8_t*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel };
		std::uint32_t pixel_data = *(std::uint32_t*)pixel; //typecast and dereference
		SDL_Color colour{ 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };
		SDL_GetRGB(pixel_data, surface->format, &colour.r, &colour.g, &colour.b);

		/*The individual color components(r, g, b, and a) are packed into a 32 - bit integer, following the 0xAARRGGBB format.

		colour.r is shifted to the highest 8 bits(left - shift by 24), colour.g is shifted by 16, colour.b is shifted by 8, and colour.a stays in the lowest 8 bits.

		The | operator combines these shifted values into a single 32 - bit value.

		The result is returned as the packed 32 - bit color value.*/
		return (static_cast<std::uint32_t>(colour.a) << 24) |  // Alpha
			(static_cast<std::uint32_t>(colour.r) << 16) |  // Red
			(static_cast<std::uint32_t>(colour.g) << 8) |   // Green
			static_cast<std::uint32_t>(colour.b);
	}

	vector::Vector2d<int> get_display_width_height()
	{
		return {
			display_mode.w,
			display_mode.h
		};
	}

	bool initialize_window()
	{
		// Initialize the SDL library
		// SDL_INIT_EVERYTHING flag initializes audio, video, controller etc subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cout << "Error initializing SLD.\n";
			return false;
		}
		// Get the current display mode so we can figure out current display dimensions
		if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0)
		{
			std::cout << "Error getting SLD Display mode.\n";
			return false;
		}
		// Create the SDL Window
		window = SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			display_mode.w,
			display_mode.h,
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
		renderer = SDL_CreateRenderer(window, -1, 0);

		if (!renderer)
		{
			std::cout << "Error creating SDL renderer.\n";
			return false;
		}

		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		return true;
	}

	vector::Vector4d project_vec4d(const matrix::Matrix4x4& projection_matrix, const vector::Vector4d& vec4d)
	{
		// apply projection matrix
		vector::Vector4d projected_point{ projection_matrix.project(vec4d) };

		//scale first
		projected_point.m_x *= display_mode.w / 2;
		projected_point.m_y *= display_mode.h / 2;
		// invert the y values to account for flipped screen y coordinate
		// in the model, y values increase as we go up, while on the screen the y increases as we go down
		projected_point.m_y *= -1.0;
		// then translate
		projected_point.m_x += display_mode.w / 2;
		projected_point.m_y += display_mode.h / 2;
		return projected_point;
	}

	void render_colour_buffer(std::uint32_t*& colour_buffer)
	{
		// update the texture with the contents of the colour buffer
		SDL_UpdateTexture(
			colour_buffer_texture,
			NULL,
			colour_buffer,
			static_cast<int>(display_mode.w * sizeof(std::uint32_t))
		);
		// copy the texture onto the renderer, null, null copies entire texture
		SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL);
	}

	void render_present()
	{
		SDL_RenderPresent(renderer);
	}

	bool setup()
	{
		// first create the window and rendering context
		bool initialized = initialize_window();
		if (initialized) {
			// Create a texture for a rendering context. ie to display the colour buffer
			colour_buffer_texture = SDL_CreateTexture(
				renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				display_mode.w,
				display_mode.h
			);
		}
		return initialized;
	}
}