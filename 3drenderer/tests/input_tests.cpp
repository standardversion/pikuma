#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../sdlwrapper.h"
#include "sdl_mock.h"
#include "../input.h"
#include <SDL.h>

namespace input_tests
{
	MockSDL mock_sdl{};
	SDL_Event event{};
	input::Input input{};

	TEST(input_test, process_sdl_quit_event)
	{
		bool is_running{ true };
		bool backface_culling{ false };
		int render_mode{ 0 };
		event.type = SDL_QUIT;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));
		
		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(is_running, false);
	}

	TEST(input_test, process_sdl_esc_key)
	{
		bool is_running{ true };
		bool backface_culling{ false };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_ESCAPE;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(is_running, false);
	}

	TEST(input_test, process_sdl_c_key)
	{
		bool is_running{ true };
		bool backface_culling{ false };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_c;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(backface_culling, true);
	}

	TEST(input_test, process_sdl_b_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_b;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(backface_culling, false);
	}

	TEST(input_test, process_sdl_0_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 1 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_0;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 0);
	}

	TEST(input_test, process_sdl_1_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_1;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 1);
	}

	TEST(input_test, process_sdl_2_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_2;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 2);
	}

	TEST(input_test, process_sdl_3_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_3;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 3);
	}

	TEST(input_test, process_sdl_4_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_4;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 4);
	}

	TEST(input_test, process_sdl_5_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_5;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 5);
	}

	TEST(input_test, process_sdl_6_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_6;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 6);
	}

	TEST(input_test, process_sdl_7_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_7;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 7);
	}

	TEST(input_test, process_sdl_8_key)
	{
		bool is_running{ true };
		bool backface_culling{ true };
		int render_mode{ 0 };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_8;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, render_mode, backface_culling, event, mock_sdl);
		EXPECT_EQ(render_mode, 8);
	}
}