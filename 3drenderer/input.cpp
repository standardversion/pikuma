#include "input.h"

namespace input
{
	void process(
		bool& is_running,
		int& render_mode,
		bool& backface_culling,
		bool& render_flat_shaded,
		bool& render_gouraud_shaded,
		bool& render_texture,
		SDL_Event& event
	)
	{
		
		SDL_PollEvent(&event);
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
			else if (event.key.keysym.sym == SDLK_5)
				render_mode = 5;
			else if (event.key.keysym.sym == SDLK_6)
				render_mode = 6;
			else if (event.key.keysym.sym == SDLK_7)
				render_mode = 7;
			else if (event.key.keysym.sym == SDLK_8)
				render_mode = 8;
			else if (event.key.keysym.sym == SDLK_c)
				backface_culling = true;
			else if (event.key.keysym.sym == SDLK_b)
				backface_culling = false;
			else if (event.key.keysym.sym == SDLK_g)
			{
				render_gouraud_shaded = true;
				render_flat_shaded = false;
			}
			else if (event.key.keysym.sym == SDLK_f)
			{
				render_gouraud_shaded = false;
				render_flat_shaded = true;
			}
			else if (event.key.keysym.sym == SDLK_t)
			{
				render_texture = !render_texture;
			}
			break;
		}
	}
}