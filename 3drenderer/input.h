#pragma once
#include <SDL.h>

namespace input
{
	void process(
		bool& is_running,
		int& render_mode,
		bool& backface_culling,
		bool& render_flat_shaded,
		bool& render_gourand_shaded,
		bool& render_texture,
		SDL_Event& event
	);
}