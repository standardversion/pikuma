#include "display.h"
#include "utils.h"

namespace display
{
	void Display::activate_render_mode(const int render_mode, bool& render_wireframe, bool& render_vertex, bool& render_shaded, bool& render_face_center, bool& render_normals) const
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

	std::uint32_t Display::apply_light_intensity(const std::uint32_t colour, double percentage_factor) const
	{
		if (percentage_factor < 0) percentage_factor = 0;
		if (percentage_factor > 1) percentage_factor = 1;
		const std::uint32_t a = (colour & 0xFF000000);
		const std::uint32_t r = (colour & 0x00FF0000) * percentage_factor;
		const std::uint32_t g = (colour & 0x0000FF00) * percentage_factor;
		const std::uint32_t b = (colour & 0x000000FF) * percentage_factor;

		return std::uint32_t{ a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF) };
	}

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

	void Display::draw_line(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, double start_i, double end_i, const std::uint32_t colour) const
	{
		int delta_x{ x1 - x0 };
		int delta_y{ y1 - y0 };

		int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);
		// Find out how much we should increment in x and y in each iteration
		double x_increment = delta_x / static_cast<double>(side_length);
		double y_increment = delta_y / static_cast<double>(side_length);

		double current_x{ static_cast<double>(x0) };
		double current_y{ static_cast<double>(y0) };
		double scalar_factor{ 0 };
		double light_intensity{ start_i };
		for (int i{ 0 }; i <= side_length; i++)
		{
			if (i == side_length) {
				light_intensity = end_i;
			} else if (i == 0) { 
				light_intensity = start_i;
			}
			else {
				light_intensity = start_i + (end_i - start_i) * scalar_factor;
			}

			std::uint32_t pixel_colour{ apply_light_intensity(colour, light_intensity) };
			draw_pixel(colour_buffer, display_mode, round(current_x), round(current_y), pixel_colour);
			current_x += x_increment;
			current_y += y_increment;
			scalar_factor = vector::Vector2d<int>::get_scalar_factor({ x0, y0 }, { static_cast<int>(current_x), static_cast<int>(current_y) }, { x1, y1 });
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

	void Display::draw_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour) const
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

	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled triangle with the flat-top/flat-bottom method
	// We split the original triangle in two, half flat-bottom and half flat-top
	///////////////////////////////////////////////////////////////////////////////
	//
	//          (x0,y0)
	//            / \
	//           /   \
	//          /     \
	//         /       \
	//        /         \
	//   (x1,y1)------(Mx,My)
	//       \_           \
	//          \_         \
	//             \_       \
	//                \_     \
	//                   \    \
	//                     \_  \
	//                        \_\
	//                           \
	//                         (x2,y2)
	//
	///////////////////////////////////////////////////////////////////////////////
	void Display::fill_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gourand_shaded, geo::Triangle<int>& triangle, const std::uint32_t colour) const
	{
		// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
		triangle.sort_vertices_by_y();

		if (triangle.m_points[1].m_y == triangle.m_points[2].m_y)
		{
			fill_flat_bottom_triangle(colour_buffer, display_mode, render_flat_shaded, render_gourand_shaded, triangle, colour);
		}
		else if (triangle.m_points[0].m_y == triangle.m_points[1].m_y)
		{
			fill_flat_top_triangle(colour_buffer, display_mode, render_flat_shaded, render_gourand_shaded, triangle, colour);
		}
		else {
			vector::Vector2d<int> midpoint{ triangle.get_midpoint() };
			double scalar_factor{ vector::Vector2d<int>::get_scalar_factor(triangle.m_points[0], midpoint, triangle.m_points[2]) };
			double mp_intensity{ triangle.m_per_vtx_lt_intensity[0] + (triangle.m_per_vtx_lt_intensity[2] - triangle.m_per_vtx_lt_intensity[0]) * scalar_factor };
			geo::Triangle<int> flat_bottom_triangle{ triangle.m_points[0], triangle.m_points[1], midpoint };
			flat_bottom_triangle.m_light_intensity = triangle.m_light_intensity;
			flat_bottom_triangle.m_per_vtx_lt_intensity.push_back(triangle.m_per_vtx_lt_intensity[0]);
			flat_bottom_triangle.m_per_vtx_lt_intensity.push_back(triangle.m_per_vtx_lt_intensity[1]);
			flat_bottom_triangle.m_per_vtx_lt_intensity.push_back(mp_intensity);
			geo::Triangle<int> flat_top_triangle{ triangle.m_points[1], midpoint, triangle.m_points[2] };
			flat_top_triangle.m_light_intensity = triangle.m_light_intensity;
			flat_top_triangle.m_per_vtx_lt_intensity.push_back(triangle.m_per_vtx_lt_intensity[1]);
			flat_top_triangle.m_per_vtx_lt_intensity.push_back(mp_intensity);
			flat_top_triangle.m_per_vtx_lt_intensity.push_back(triangle.m_per_vtx_lt_intensity[2]);
			fill_flat_bottom_triangle(colour_buffer, display_mode, render_flat_shaded, render_gourand_shaded, flat_bottom_triangle, colour);
			fill_flat_top_triangle(colour_buffer, display_mode, render_flat_shaded, render_gourand_shaded, flat_top_triangle, colour);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled a triangle with a flat bottom
	///////////////////////////////////////////////////////////////////////////////
	//
	//        (x0,y0) a
	//          / \
	//         /   \
	//        /     \
	//       /       \
	//      /         \
	// b (x1,y1)------(x2,y2) c
	//
	///////////////////////////////////////////////////////////////////////////////
	void Display::fill_flat_bottom_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gourand_shaded, const geo::Triangle<int>& triangle, const std::uint32_t colour) const
	{
		double x_start_slope{ triangle.get_inverse_slope(1, 0) };
		double x_end_slope{ triangle.get_inverse_slope(2, 0) };
		double x_start{ static_cast<double>(triangle.m_points[0].m_x) };
		double x_end{ static_cast<double>(triangle.m_points[0].m_x) };
		double start_intensity{ triangle.m_per_vtx_lt_intensity[0] };
		double end_intensity{ triangle.m_per_vtx_lt_intensity[0] };
		double scalar_factor_ab{ 0 };
		double scalar_factor_ac{ 0 };
		const std::uint32_t light_colour{ apply_light_intensity(colour, triangle.m_light_intensity) };
		for (int i{ triangle.m_points[0].m_y }; i <= triangle.m_points[1].m_y; i++)
		{
			if (render_gourand_shaded)
			{
				draw_line(
					colour_buffer,
					display_mode,
					x_start,
					i,
					x_end,
					i,
					start_intensity,
					end_intensity,
					colour
				);
			}
			if (render_flat_shaded)
			{
				draw_line(
					colour_buffer,
					display_mode,
					x_start,
					i,
					x_end,
					i,
					light_colour
				);
			}
			x_start += x_start_slope;
			x_end += x_end_slope;
			scalar_factor_ab = vector::Vector2d<int>::get_scalar_factor(triangle.m_points[0], { static_cast<int>(x_start), static_cast<int>(i) }, triangle.m_points[1]);
			start_intensity = triangle.m_per_vtx_lt_intensity[0] + (triangle.m_per_vtx_lt_intensity[1] - triangle.m_per_vtx_lt_intensity[0]) * scalar_factor_ab;
			scalar_factor_ac = vector::Vector2d<int>::get_scalar_factor(triangle.m_points[0], { static_cast<int>(x_end), static_cast<int>(i) }, triangle.m_points[2]);
			end_intensity = triangle.m_per_vtx_lt_intensity[0] + (triangle.m_per_vtx_lt_intensity[2] - triangle.m_per_vtx_lt_intensity[0]) * scalar_factor_ab;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Draw a filled a triangle with a flat top
	///////////////////////////////////////////////////////////////////////////////
	//
	// a (x0,y0)------(x1,y1) b
	//      \         /
	//       \       /
	//        \     /
	//         \   /
	//          \ /
	//        (x2,y2) c
	//
	///////////////////////////////////////////////////////////////////////////////
	void Display::fill_flat_top_triangle(std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const bool render_flat_shaded, const bool render_gourand_shaded, const geo::Triangle<int>& triangle, const std::uint32_t colour) const
	{
		double x_start_slope{ triangle.get_inverse_slope(2, 0) };
		double x_end_slope{ triangle.get_inverse_slope(2, 1) };
		double x_start{ static_cast<double>(triangle.m_points[2].m_x) };
		double x_end{ static_cast<double>(triangle.m_points[2].m_x) };
		double start_intensity{ triangle.m_per_vtx_lt_intensity[2] };
		double end_intensity{ triangle.m_per_vtx_lt_intensity[2] };
		double scalar_factor_ac{ 0 };
		double scalar_factor_bc{ 0 };
		const std::uint32_t light_colour{ apply_light_intensity(colour, triangle.m_light_intensity) };
		for (int i{ triangle.m_points[2].m_y }; i >= triangle.m_points[0].m_y; i--)
		{
			if (render_gourand_shaded)
			{
				draw_line(
					colour_buffer,
					display_mode,
					x_start,
					i,
					x_end,
					i,
					start_intensity,
					end_intensity,
					colour
				);
			}
			if (render_flat_shaded)
			{
				draw_line(
					colour_buffer,
					display_mode,
					x_start,
					i,
					x_end,
					i,
					light_colour
				);
			}
			x_start -= x_start_slope;
			x_end -= x_end_slope;
			scalar_factor_ac = vector::Vector2d<int>::get_scalar_factor(triangle.m_points[0], { static_cast<int>(x_start), static_cast<int>(i) }, triangle.m_points[2]);
			start_intensity = triangle.m_per_vtx_lt_intensity[0] + (triangle.m_per_vtx_lt_intensity[2] - triangle.m_per_vtx_lt_intensity[0]) * scalar_factor_ac;
			scalar_factor_bc = vector::Vector2d<int>::get_scalar_factor(triangle.m_points[1], { static_cast<int>(x_end), static_cast<int>(i) }, triangle.m_points[2]);
			end_intensity = triangle.m_per_vtx_lt_intensity[1] + (triangle.m_per_vtx_lt_intensity[2] - triangle.m_per_vtx_lt_intensity[1]) * scalar_factor_bc;
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

	vector::Vector2d<double> Display::project_vec4d(const SDL_DisplayMode* display_mode, const matrix::Matrix4x4& projection_matrix, const vector::Vector4d& vec4d) const
	{
		// apply project matrix
		vector::Vector4d projected_vertex{ projection_matrix.project(vec4d) };
		vector::Vector2d<double> projected_point{ projected_vertex.m_x, projected_vertex.m_y };

		//scale first
		projected_point.m_x *= display_mode->w / 2;
		projected_point.m_y *= display_mode->h / 2;
		// invert the y values to account for flipped screen y coordinate
		// in the model, y values increase as we go up, while on the screen the y increases as we go down
		projected_point.m_y *= -1.0;
		// then translate
		projected_point.m_x += display_mode->w / 2;
		projected_point.m_y += display_mode->h / 2;
		return projected_point;
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