#include "input.h"

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
		bool& render_texture
	)
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
				is_running = false;
			if (event.key.keysym.sym == SDLK_0)
				render_mode = 0;
			if (event.key.keysym.sym == SDLK_1)
				render_mode = 1;
			if (event.key.keysym.sym == SDLK_2)
				render_mode = 2;
			if (event.key.keysym.sym == SDLK_3)
				render_mode = 3;
			if (event.key.keysym.sym == SDLK_4)
				render_mode = 4;
			if (event.key.keysym.sym == SDLK_5)
				render_mode = 5;
			if (event.key.keysym.sym == SDLK_6)
				render_mode = 6;
			if (event.key.keysym.sym == SDLK_7)
				render_mode = 7;
			if (event.key.keysym.sym == SDLK_8)
				render_mode = 8;
			if (event.key.keysym.sym == SDLK_c)
				backface_culling = true;
			if (event.key.keysym.sym == SDLK_b)
				backface_culling = false;
			if (event.key.keysym.sym == SDLK_g)
			{
				render_gouraud_shaded = true;
				render_flat_shaded = false;
			}
			if (event.key.keysym.sym == SDLK_f)
			{
				render_gouraud_shaded = false;
				render_flat_shaded = true;
			}
			if (event.key.keysym.sym == SDLK_t)
			{
				render_texture = !render_texture;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				view_camera.m_position.m_y += 3.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				view_camera.m_position.m_y -= 3.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				view_camera.m_yaw += 1.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				view_camera.m_yaw -= 1.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				view_camera.m_pitch += 1.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				view_camera.m_pitch -= 1.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_w)
			{
				view_camera.m_forward_velocity = view_camera.m_direction * (5.0 * delta_time);
				view_camera.m_position += view_camera.m_forward_velocity;
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				view_camera.m_forward_velocity = view_camera.m_direction * (5.0 * delta_time);
				view_camera.m_position -= view_camera.m_forward_velocity;
			}
			if (event.key.keysym.sym == SDLK_l)
			{
				
				display::toggle_make_low_rez();
			}
			break;
		}
	}
}