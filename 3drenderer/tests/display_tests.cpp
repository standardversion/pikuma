#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../sdlwrapper.h"
#include "sdl_mock.h"
#include "../display.h"
#include <iostream>

namespace display_tests {
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* colour_buffer_texture{ nullptr };
	SDL_DisplayMode display_mode;
	MockSDL mock_sdl;
	TEST(display_test, initialize_window_sdl_init_fail)
	{
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(1));

		bool initialized = display::initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_sdl_display_mode_fail)
	{
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(1));

		bool initialized = display::initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_sdl_create_win_fail)
	{
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS))
			.WillOnce(::testing::Return(nullptr));

		bool initialized = display::initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_sdl_create_ren_fail)
	{
		SDL_Window* w_ptr{ SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS) };
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS))
			.WillOnce(::testing::Return(w_ptr));
		EXPECT_CALL(mock_sdl, SDL_CreateRenderer(w_ptr, -1, 0))
			.WillOnce(::testing::Return(nullptr));

		bool initialized = display::initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, initialize_window_success)
	{
		SDL_Window* w_ptr{ SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS) };
		SDL_Renderer* r_ptr{ SDL_CreateRenderer(w_ptr, -1, 0) };
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS))
			.WillOnce(::testing::Return(w_ptr));
		EXPECT_CALL(mock_sdl, SDL_CreateRenderer(w_ptr, -1, 0))
			.WillOnce(::testing::Return(r_ptr));
		EXPECT_CALL(mock_sdl, SDL_SetWindowFullscreen(w_ptr, SDL_WINDOW_FULLSCREEN))
			.WillOnce(::testing::Return(0));

		bool initialized = display::initialize_window(window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, true);
	}

	TEST(display_test, setup_init_fail)
	{
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(1));
		
		bool initialized = display::setup(colour_buffer_texture, window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, false);
	}

	TEST(display_test, setup_init_success)
	{
		SDL_Window* w_ptr{ SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS) };
		SDL_Renderer* r_ptr{ SDL_CreateRenderer(w_ptr, -1, 0) };
		EXPECT_CALL(mock_sdl, SDL_Init(SDL_INIT_EVERYTHING))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_GetCurrentDisplayMode(0, &display_mode))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_BORDERLESS))
			.WillOnce(::testing::Return(w_ptr));
		EXPECT_CALL(mock_sdl, SDL_CreateRenderer(w_ptr, -1, 0))
			.WillOnce(::testing::Return(r_ptr));
		EXPECT_CALL(mock_sdl, SDL_SetWindowFullscreen(w_ptr, SDL_WINDOW_FULLSCREEN))
			.WillOnce(::testing::Return(0));

		bool initialized = display::setup(colour_buffer_texture, window, renderer, &display_mode, mock_sdl);
		EXPECT_EQ(initialized, true);
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

		display::cleanup(w_ptr, r_ptr, colour_buffer, mock_sdl);
		EXPECT_EQ(w_ptr, nullptr);
		EXPECT_EQ(r_ptr, nullptr);
		EXPECT_EQ(colour_buffer, nullptr);
	}

	TEST(display_test, render_colour_buffer_success)
	{
		std::uint32_t* colour_buffer{ new std::uint32_t[1]{10} };
		EXPECT_CALL(mock_sdl, SDL_UpdateTexture(colour_buffer_texture, NULL, colour_buffer, static_cast<int>(display_mode.w * sizeof(std::uint32_t))))
			.WillOnce(::testing::Return(0));
		EXPECT_CALL(mock_sdl, SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL))
			.WillOnce(::testing::Return(0));

		display::render_colour_buffer(colour_buffer_texture, colour_buffer, &display_mode, renderer, mock_sdl);
		delete[] colour_buffer;
		colour_buffer = nullptr;
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

		display::draw_grid(colour_buffer, &display_mode, ln, bg, 2);
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

		display::clear_colour_buffer(colour_buffer, &display_mode, bg);
		for (int y{ 0 }; y < 4; y++)
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
			bg, bg, bg, bg, bg,
			bg, ln, ln, ln, bg,
			bg, ln, ln, ln, bg,
			bg, ln, ln, ln, bg,
			bg, bg, bg, bg, bg,
		} };

		display::draw_rect(colour_buffer, &display_mode, 2, 2, 3, 3, ln);
		for (int y{ 0 }; y < 4; y++)
		{
			EXPECT_EQ(colour_buffer[y], expected_colour_buffer[y]);
		}
		delete[] colour_buffer;
		colour_buffer = nullptr;
		delete[] expected_colour_buffer;
		expected_colour_buffer = nullptr;
	}
}
