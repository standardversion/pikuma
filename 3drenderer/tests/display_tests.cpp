#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../sdlwrapper.h"
#include "sdl_mock.h"
#include "../display.h"
#include "../vector2d.h"
#include <SDL.h>

namespace display_tests
{
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* colour_buffer_texture{ nullptr };
	SDL_DisplayMode display_mode;
	MockSDL mock_sdl{};
	display::Display dsp{};

	TEST(display_test, activate_render_modes)
	{
		bool render_wireframe{ false };
		bool render_vertex{ false };
		bool render_shaded{ false };

		dsp.activate_render_mode(0, render_wireframe, render_vertex, render_shaded);
		EXPECT_EQ(render_wireframe, true);
		EXPECT_EQ(render_vertex, false);
		EXPECT_EQ(render_shaded, false);
		dsp.activate_render_mode(1, render_wireframe, render_vertex, render_shaded);
		EXPECT_EQ(render_wireframe, true);
		EXPECT_EQ(render_vertex, true);
		EXPECT_EQ(render_shaded, false);
		dsp.activate_render_mode(2, render_wireframe, render_vertex, render_shaded);
		EXPECT_EQ(render_wireframe, false);
		EXPECT_EQ(render_vertex, false);
		EXPECT_EQ(render_shaded, true);
		dsp.activate_render_mode(3, render_wireframe, render_vertex, render_shaded);
		EXPECT_EQ(render_wireframe, true);
		EXPECT_EQ(render_vertex, false);
		EXPECT_EQ(render_shaded, true);
		dsp.activate_render_mode(4, render_wireframe, render_vertex, render_shaded);
		EXPECT_EQ(render_wireframe, true);
		EXPECT_EQ(render_vertex, true);
		EXPECT_EQ(render_shaded, true);
	}

	TEST(display_test, cleanup_success)
	{
		SDL_Window* w_ptr{ SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS) };
		SDL_Renderer* r_ptr{ SDL_CreateRenderer(w_ptr, -1, 0) };
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		EXPECT_CALL(mock_sdl, SDL_DestroyRenderer(r_ptr))
			.WillOnce(::testing::Return());
		EXPECT_CALL(mock_sdl, SDL_DestroyWindow(w_ptr))
			.WillOnce(::testing::Return());
		EXPECT_CALL(mock_sdl, SDL_Quit())
			.WillOnce(::testing::Return());

		dsp.cleanup(w_ptr, r_ptr, colour_buffer, mock_sdl);
		EXPECT_EQ(w_ptr, nullptr);
		EXPECT_EQ(r_ptr, nullptr);
		EXPECT_EQ(colour_buffer, nullptr);
	}

	TEST(display_test, clear_colour_buffer_success)
	{
		display_mode.w = 2;
		display_mode.h = 2;
		std::uint32_t bg{ 0x00000000 };
		std::uint32_t ln{ 0xFFFFFFFF };
		std::uint32_t* colour_buffer{ new std::uint32_t[4]{
			ln, ln, ln, ln
		} };
		std::uint32_t* expected_colour_buffer{ new std::uint32_t[4]{
			bg, bg, bg, bg
		} };

		dsp.clear_colour_buffer(colour_buffer, &display_mode, bg);
		for (int y{ 0 }; y < 4; y++)
		{
			EXPECT_EQ(colour_buffer[y], expected_colour_buffer[y]);
		}
		delete[] colour_buffer;
		colour_buffer = nullptr;
		delete[] expected_colour_buffer;
		expected_colour_buffer = nullptr;
	}

	TEST(display_test, draw_grid_success)
	{
		display_mode.w = 5;
		display_mode.h = 5;
		std::uint32_t* colour_buffer{ new std::uint32_t[25]{} };
		std::uint32_t bg{ 0x00000000 };
		std::uint32_t ln{ 0xFFFFFFFF };
		std::uint32_t* expected_colour_buffer{ new std::uint32_t[25]{
			bg, ln, bg, ln, bg,
			ln, ln, ln, ln, ln,
			bg, ln, bg, ln, bg,
			ln, ln, ln, ln, ln,
			bg, ln, bg, ln, bg,
		} };

		dsp.draw_grid(colour_buffer, &display_mode, ln, bg, 2);
		//::testing::internal::CaptureStdout();  // Capture stdout
		for (int y{ 0 }; y < 25; y++)
		{
			EXPECT_EQ(colour_buffer[y], expected_colour_buffer[y]);
			//std::cout << colour_buffer[y] << ":" << expected_colour_buffer[y] << std::endl << std::flush;
		}
		//std::string output = ::testing::internal::GetCapturedStdout();
		//std::cout << "Captured Output: " << std::endl << output << std::endl;  // Print captured output
		delete[] colour_buffer;
		colour_buffer = nullptr;
		delete[] expected_colour_buffer;
		expected_colour_buffer = nullptr;
	}

	TEST(display_test, draw_line_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		std::uint32_t ln{ 0xFFFFFFFF };
		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(void, draw_pixel, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x, int y, const std::uint32_t colour), (const, override));
		};
		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, draw_pixel(colour_buffer, &display_mode, 1, 1, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_pixel(colour_buffer, &display_mode, 2, 2, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_pixel(colour_buffer, &display_mode, 3, 3, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_pixel(colour_buffer, &display_mode, 4, 4, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_pixel(colour_buffer, &display_mode, 5, 5, ln))
			.Times(1);

		mock_dsp.draw_line(colour_buffer, &display_mode, 1, 1, 5, 5, ln);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, draw_pixel_within_window)
	{
		display_mode.w = 5;
		display_mode.h = 5;
		std::uint32_t bg{ 0x00000000 };
		std::uint32_t cl{ 0xFFFFFFFF };
		std::uint32_t* colour_buffer{ new std::uint32_t[25]{
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
		} };
		std::uint32_t* expected_colour_buffer{ new std::uint32_t[25]{
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
		} };

		dsp.draw_pixel(colour_buffer, &display_mode, 10, 20, cl);
		for (int y{ 0 }; y < 25; y++)
		{
			EXPECT_EQ(colour_buffer[y], expected_colour_buffer[y]);
		}
		delete[] colour_buffer;
		colour_buffer = nullptr;
		delete[] expected_colour_buffer;
		expected_colour_buffer = nullptr;
	}

	TEST(display_test, draw_pixel_success)
	{
		display_mode.w = 5;
		display_mode.h = 5;
		std::uint32_t bg{ 0x00000000 };
		std::uint32_t cl{ 0xFFFFFFFF };
		std::uint32_t* colour_buffer{ new std::uint32_t[25]{
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
		} };
		std::uint32_t* expected_colour_buffer{ new std::uint32_t[25]{
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, cl, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
		} };
		dsp.draw_pixel(colour_buffer, &display_mode, 1, 2, cl);
		for (int y{ 0 }; y < 25; y++)
		{
			EXPECT_EQ(colour_buffer[y], expected_colour_buffer[y]);
		}
		delete[] colour_buffer;
		colour_buffer = nullptr;
		delete[] expected_colour_buffer;
		expected_colour_buffer = nullptr;
	}

	TEST(display_test, draw_rect_success)
	{
		display_mode.w = 5;
		display_mode.h = 5;
		std::uint32_t bg{ 0x00000000 };
		std::uint32_t ln{ 0xFFFFFFFF };
		std::uint32_t* colour_buffer{ new std::uint32_t[25]{
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
		} };
		std::uint32_t* expected_colour_buffer{ new std::uint32_t[25]{
			ln, ln, bg, bg, bg,
			ln, ln, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
			bg, bg, bg, bg, bg,
		} };

		dsp.draw_rect(colour_buffer, &display_mode, 0, 0, 2, 2, ln);
		for (int y{ 0 }; y < 25; y++)
		{
			EXPECT_EQ(colour_buffer[y], expected_colour_buffer[y]);
		}
		delete[] colour_buffer;
		colour_buffer = nullptr;
		delete[] expected_colour_buffer;
		expected_colour_buffer = nullptr;
	}

	TEST(display_test, draw_triangle_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		std::uint32_t ln{ 0xFFFFFFFF };
		std::vector <vector::Vector2d<int>> points{
			{1, 2},
			{ 10, 2 },
			{ 10, 5 }
		};
		geo::Triangle<int> triangle{
			points
		};
		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(void, draw_line, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const std::uint32_t colour), (const, override));
		};
		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 1, 2, 10, 2, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 1, 2, 10, 5, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 10, 2, 10, 5, ln))
			.Times(1);

		mock_dsp.draw_triangle(colour_buffer, &display_mode, triangle, ln);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, fill_triangle_only_flat_bottom_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		std::uint32_t fill{ 0xFFFFFFFF };
		std::vector <vector::Vector2d<int>> points{
			{1, 2},
			{ 10, 5 },
			{ 10, 5 }
		};
		geo::Triangle<int> triangle{
			points
		};

		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(void, fill_flat_bottom_triangle, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour), (const, override));
			MOCK_METHOD(void, fill_flat_top_triangle, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour), (const, override));
		};


		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, fill_flat_bottom_triangle(colour_buffer, &display_mode, ::testing::_, fill))
			.Times(1);
		EXPECT_CALL(mock_dsp, fill_flat_top_triangle(colour_buffer, &display_mode, ::testing::_, fill))
			.Times(0);

		mock_dsp.fill_triangle(colour_buffer, &display_mode, triangle, fill);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, fill_triangle_only_flat_top_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		std::uint32_t fill{ 0xFFFFFFFF };
		std::vector <vector::Vector2d<int>> points{
			{1, 2},
			{ 10, 2 },
			{ 10, 5 }
		};
		geo::Triangle<int> triangle{
			points
		};

		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(void, fill_flat_bottom_triangle, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour), (const, override));
			MOCK_METHOD(void, fill_flat_top_triangle, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour), (const, override));
		};


		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, fill_flat_bottom_triangle(colour_buffer, &display_mode, ::testing::_, fill))
			.Times(0);
		EXPECT_CALL(mock_dsp, fill_flat_top_triangle(colour_buffer, &display_mode, ::testing::_, fill))
			.Times(1);

		mock_dsp.fill_triangle(colour_buffer, &display_mode, triangle, fill);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, fill_triangle_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		std::uint32_t fill{ 0xFFFFFFFF };
		std::vector <vector::Vector2d<int>> points{
			{1, 2},
			{ 10, 7 },
			{ 10, 5 }
		};
		geo::Triangle<int> triangle{
			points
		};

		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(void, fill_flat_bottom_triangle, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour), (const, override));
			MOCK_METHOD(void, fill_flat_top_triangle, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, const geo::Triangle<int>& triangle, const std::uint32_t colour), (const, override));
		};


		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, fill_flat_bottom_triangle(colour_buffer, &display_mode, ::testing::_, fill))
			.Times(1);
		EXPECT_CALL(mock_dsp, fill_flat_top_triangle(colour_buffer, &display_mode, ::testing::_, fill))
			.Times(1);

		mock_dsp.fill_triangle(colour_buffer, &display_mode, triangle, fill);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, fill_flat_bottom_triangle_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		std::uint32_t ln{ 0xFFFFFFFF };
		std::vector <vector::Vector2d<int>> points{
			{0, 0},
			{ 10, 5 },
			{ 20, 5 }
		};
		geo::Triangle<int> triangle{
			points
		};
		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(void, draw_line, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const std::uint32_t colour), (const, override));
		};
		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 0.0, 0, 0.0, 0, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 2.0, 1, 4.0, 1, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 4.0, 2, 8.0, 2, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 6.0, 3, 12.0, 3, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 8.0, 4, 16.0, 4, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 10.0, 5, 20.0, 5, ln))
			.Times(1);

		mock_dsp.fill_flat_bottom_triangle(colour_buffer, &display_mode, triangle, ln);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, fill_flat_top_triangle_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		std::uint32_t ln{ 0xFFFFFFFF };
		std::vector <vector::Vector2d<int>> points{
			{ 10, 5 },
			{ 20, 5 },
			{ 15, 10 },
		};
		geo::Triangle<int> triangle{
			points
		};
		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(void, draw_line, (std::uint32_t*& colour_buffer, const SDL_DisplayMode* display_mode, int x0, int y0, int x1, int y1, const std::uint32_t colour), (const, override));
		};
		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 15.0, 10, 15.0, 10, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 14.0, 9, 16.0, 9, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 13.0, 8, 17.0, 8, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 12.0, 7, 18.0, 7, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 11.0, 6, 19.0, 6, ln))
			.Times(1);
		EXPECT_CALL(mock_dsp, draw_line(colour_buffer, &display_mode, 10.0, 5, 20.0, 5, ln))
			.Times(1);

		mock_dsp.fill_flat_top_triangle(colour_buffer, &display_mode, triangle, ln);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, initialize_window_sdl_init_fail)
	{
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(1));

		bool initialized = dsp.initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_sdl_display_mode_fail)
	{
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(1));

		bool initialized = dsp.initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_sdl_create_win_fail)
	{
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ::testing::_, ::testing::_, SDL_WINDOW_BORDERLESS))
			.WillOnce(::testing::Return(nullptr));

		bool initialized = dsp.initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_sdl_create_ren_fail)
	{
		SDL_Window* w_ptr{ SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display_mode.w, display_mode.h, SDL_WINDOW_BORDERLESS) };
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ::testing::_, ::testing::_, SDL_WINDOW_BORDERLESS))
			.WillOnce(::testing::Return(w_ptr));
		EXPECT_CALL(mock_sdl, SDL_CreateRenderer(w_ptr, -1, 0))
			.WillOnce(::testing::Return(nullptr));

		bool initialized = dsp.initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_success)
	{
		SDL_Window* w_ptr{ SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display_mode.w, display_mode.h, SDL_WINDOW_BORDERLESS) };
		SDL_Renderer* r_ptr{ SDL_CreateRenderer(w_ptr, -1, 0) };
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ::testing::_, ::testing::_, SDL_WINDOW_BORDERLESS))
			.WillOnce(::testing::Return(w_ptr));
		EXPECT_CALL(mock_sdl, SDL_CreateRenderer(w_ptr, -1, 0))
			.WillOnce(::testing::Return(r_ptr));
		EXPECT_CALL(mock_sdl, SDL_SetWindowFullscreen(w_ptr, SDL_WINDOW_FULLSCREEN))
			.WillOnce(::testing::Return(0));

		bool initialized = dsp.initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, true);
	}

	TEST(display_test, render_colour_buffer_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		EXPECT_CALL(mock_sdl, SDL_UpdateTexture(colour_buffer_texture, NULL, colour_buffer, static_cast<int>(display_mode.w * sizeof(std::uint32_t))))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL))
			.WillOnce(::testing::Return(0));

		dsp.render_colour_buffer(colour_buffer_texture, colour_buffer, &display_mode, renderer, mock_sdl);
		delete[] colour_buffer;
		colour_buffer = nullptr;
	}

	TEST(display_test, setup_init_fail)
	{
		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(bool, initialize_window, (SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode, const SDLWrapper& sdl), (const, override));
		};
		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, initialize_window(window, renderer, &display_mode, ::testing::_))
			.WillOnce(::testing::Return(false));

		bool initialized = mock_dsp.setup(colour_buffer_texture, window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, setup_init_success)
	{
		class MockDisplay : public display::Display
		{
		public:
			MOCK_METHOD(bool, initialize_window, (SDL_Window*& window, SDL_Renderer*& renderer, SDL_DisplayMode* display_mode, const SDLWrapper& sdl), (const, override));
		};
		MockDisplay mock_dsp;
		EXPECT_CALL(mock_dsp, initialize_window(window, renderer, &display_mode, ::testing::_))
			.WillOnce(::testing::Return(true));

		EXPECT_CALL(mock_sdl, SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, display_mode.w, display_mode.h))
			.WillOnce(::testing::Return(colour_buffer_texture));


		bool initialized = mock_dsp.setup(colour_buffer_texture, window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, true);
	}
}
