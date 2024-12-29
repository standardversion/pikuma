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
		event.type = SDL_QUIT;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));
		
		input.process(is_running, event, mock_sdl);
		EXPECT_EQ(is_running, false);
	}

	TEST(input_test, process_sdl_esc_key)
	{
		bool is_running{ true };
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = SDLK_ESCAPE;
		EXPECT_CALL(mock_sdl, SDL_PollEvent(&event))
			.WillOnce(::testing::Return(0));

		input.process(is_running, event, mock_sdl);
		EXPECT_EQ(is_running, false);
	}
}