#pragma once
#include <SDL.h>
#include "sdlwrapper.h"

namespace input
{
	class Input
	{
	public:
		virtual ~Input() = default;
		virtual void process(bool& is_running, int& render_mode, bool& backface_culling, SDL_Event& event, const SDLWrapper& sdl) const;
	};
}