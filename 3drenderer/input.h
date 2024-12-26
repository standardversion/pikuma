#pragma once
#include <SDL.h>
#include "sdlwrapper.h"

namespace input {
	void process(bool& is_running, SDLWrapper& sdl);
}