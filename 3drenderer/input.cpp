#include "input.h"

namespace input
{
	void Input::process(bool& is_running, int& render_mode, bool& backface_culling, SDL_Event& event, const SDLWrapper& sdl) const
	{
		
		sdl.SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			else if (event.key.keysym.sym == SDLK_0)
				render_mode = 0;
			else if (event.key.keysym.sym == SDLK_1)
				render_mode = 1;
			else if (event.key.keysym.sym == SDLK_2)
				render_mode = 2;
			else if (event.key.keysym.sym == SDLK_3)
				render_mode = 3;
			else if (event.key.keysym.sym == SDLK_4)
				render_mode = 4;
			else if (event.key.keysym.sym == SDLK_c)
				backface_culling = true;
			else if (event.key.keysym.sym == SDLK_b)
				backface_culling = false;
			break;
		}
	}
}