#pragma once
#include <SDL.h>
#include "camera.h"

namespace input
{
	void process(
		bool& is_running,
		camera::camera_t& view_camera,
		const double delta_time,
		int& render_mode,
		bool& backface_culling,
		bool& render_flat_shaded,
		bool& render_gouraud_shaded,
		bool& render_texture,
		SDL_Event& event
	);
}