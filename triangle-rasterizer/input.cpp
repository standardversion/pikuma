#include "input.h"

namespace input
{
	void process(bool& is_running, bool& top_left_rasterization)
	{
		SDL_Event event{};
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				is_running = false;
			}
			if (event.key.keysym.sym == SDLK_t)
			{
				top_left_rasterization = !top_left_rasterization;
			}
			break;
		}
	}
}