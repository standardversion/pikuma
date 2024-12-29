#pragma once
#include <SDL.h>
#include "sdlwrapper.h"

namespace input
{
	class Input
	{
	public:
		virtual ~Input() = default;
		virtual void process(bool& is_running, SDL_Event& event, SDLWrapper& sdl);
	};
}