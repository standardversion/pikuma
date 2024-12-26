#pragma once
#include <functional>
#include <SDL.h>
#include <SDL_stdinc.h>


// define function pointers
using CreateRenderer_Fcn = std::function<SDL_Renderer* (SDL_Window*, int, Uint32)>;
using CreateTexture_Fcn = std::function<SDL_Texture* (SDL_Renderer*, Uint32, int, int, int)>;
using CreateWindow_Fcn = std::function<SDL_Window* (const char*, int, int, int, int, Uint32)>;
using DestroyRenderer_Fcn = std::function<void(SDL_Renderer*)>;
using DestroyWindow_Fcn = std::function<void(SDL_Window*)>;
using GetCurrentDisplayMode_Fcn = std::function<int(int, SDL_DisplayMode*)>;
using Init_Fcn = std::function<int(Uint32)>;
using PollEvent_Fcn = std::function<int(SDL_Event*)>;
using Quit_Fcn = std::function<void()>;
using RenderClear_Fcn = std::function<int(SDL_Renderer*)>;
using RenderCopy_Fcn = std::function<int(SDL_Renderer*, SDL_Texture*, const SDL_Rect*, const SDL_Rect*)>;
using RenderPresent_Fcn = std::function<void(SDL_Renderer*)>;
using SetRenderDrawColour_Fcn = std::function<int(SDL_Renderer*, Uint8, Uint8, Uint8, Uint8)>;
using SetWindowFullScreen_Fcn = std::function<int(SDL_Window*, Uint32)>;
using UpdateTexture_Fcn = std::function<int(SDL_Texture*, const SDL_Rect* , const void*, int)>;

// Wrapper class for SDL so we can mock it for tests
class SDLWrapper
{
public:
	SDLWrapper(
		CreateRenderer_Fcn create_renderer_fcn,
		CreateTexture_Fcn create_texture_fcn,
		CreateWindow_Fcn create_window_fcn,
		DestroyRenderer_Fcn destroy_renderer_fcn,
		DestroyWindow_Fcn destroy_window_fcn,
		GetCurrentDisplayMode_Fcn get_current_display_mode_fcn,
		Init_Fcn init_fcn,
		PollEvent_Fcn poll_event_fcn,
		Quit_Fcn quit_fcn,
		RenderClear_Fcn render_clear_fcn,
		RenderCopy_Fcn render_copy_fcn,
		RenderPresent_Fcn render_present_fcn,
		SetRenderDrawColour_Fcn render_draw_color_fcn,
		SetWindowFullScreen_Fcn set_window_fullsrcn_fcn,
		UpdateTexture_Fcn update_texture_fcn
	);
	SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags);
	SDL_Texture* SDL_CreateTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h);
	SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
	void SDL_DestroyRenderer(SDL_Renderer* renderer);
	void SDL_DestroyWindow(SDL_Window* window);
	int SDL_GetCurrentDisplayMode(int index, SDL_DisplayMode* display_mode);
	int SDL_Init(Uint32 flags);
	int SDL_PollEvent(SDL_Event* event);
	void SDL_Quit();
	int SDL_RenderClear(SDL_Renderer* renderer);
	int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
	void SDL_RenderPresent(SDL_Renderer* renderer);
	int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	int SDL_SetWindowFullscreen(SDL_Window* window, Uint32 flags);
	int SDL_UpdateTexture(SDL_Texture* texture, const SDL_Rect* rect, const void* pixels, int pitch);

private:
	CreateRenderer_Fcn m_create_renderer{};
	CreateTexture_Fcn m_create_texture{};
	CreateWindow_Fcn m_create_window{};
	DestroyRenderer_Fcn m_destroy_renderer{};
	DestroyWindow_Fcn m_destroy_window{};
	GetCurrentDisplayMode_Fcn m_get_current_display_mode{};
	Init_Fcn m_init{};
	PollEvent_Fcn m_poll_event{};
	Quit_Fcn m_quit{};
	RenderClear_Fcn m_render_clear{};
	RenderCopy_Fcn m_render_copy{};
	RenderPresent_Fcn m_render_present{};
	SetRenderDrawColour_Fcn m_render_draw_colour{};
	SetWindowFullScreen_Fcn m_set_window_fullscreen{};
	UpdateTexture_Fcn m_update_texture{};
	
};

inline SDLWrapper::SDLWrapper(
	CreateRenderer_Fcn create_renderer_fcn,
	CreateTexture_Fcn create_texture_fcn,
	CreateWindow_Fcn create_window_fcn,
	DestroyRenderer_Fcn destroy_renderer_fcn,
	DestroyWindow_Fcn destroy_window_fcn,
	GetCurrentDisplayMode_Fcn get_current_display_mode_fcn,
	Init_Fcn init_fcn,
	PollEvent_Fcn poll_event_fcn,
	Quit_Fcn quit_fcn,
	RenderClear_Fcn render_clear_fcn,
	RenderCopy_Fcn render_copy_fcn,
	RenderPresent_Fcn render_present_fcn,
	SetRenderDrawColour_Fcn render_draw_color_fcn,
	SetWindowFullScreen_Fcn set_window_fullsrcn_fcn,
	UpdateTexture_Fcn update_texture_fcn
)
	:
	m_create_renderer{ create_renderer_fcn },
	m_create_texture{ create_texture_fcn },
	m_create_window{ create_window_fcn },
	m_destroy_renderer{ destroy_renderer_fcn },
	m_destroy_window{ destroy_window_fcn },
	m_get_current_display_mode{ get_current_display_mode_fcn },
	m_init{ init_fcn },
	m_poll_event{ poll_event_fcn },
	m_quit{ quit_fcn },
	m_render_clear{ render_clear_fcn },
	m_render_copy{ render_copy_fcn },
	m_render_present{ render_present_fcn },
	m_render_draw_colour{ render_draw_color_fcn},
	m_set_window_fullscreen{ set_window_fullsrcn_fcn },
	m_update_texture { update_texture_fcn }
{
}

inline SDL_Renderer* SDLWrapper::SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags)
{
	return m_create_renderer(window, index, flags);
}

inline SDL_Texture* SDLWrapper::SDL_CreateTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h)
{
	return m_create_texture(renderer, format, access, w, h);
}

inline SDL_Window* SDLWrapper::SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
{
	return m_create_window(title, x, y, w, h, flags);
}

inline void SDLWrapper::SDL_DestroyRenderer(SDL_Renderer* renderer)
{
	m_destroy_renderer(renderer);
}

inline void SDLWrapper::SDL_DestroyWindow(SDL_Window* window)
{
	m_destroy_window(window);
}

inline int SDLWrapper::SDL_GetCurrentDisplayMode(int index, SDL_DisplayMode* display_mode)
{
	return m_get_current_display_mode(index, display_mode);
}

inline int SDLWrapper::SDL_Init(Uint32 flags)
{
	return m_init(flags);
}

inline int SDLWrapper::SDL_PollEvent(SDL_Event* event)
{
	return m_poll_event(event);
}

inline void SDLWrapper::SDL_Quit()
{
	m_quit();
}

inline int SDLWrapper::SDL_RenderClear(SDL_Renderer* renderer)
{
	return m_render_clear(renderer);
}

inline int SDLWrapper::SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
	return m_render_copy(renderer, texture, srcrect, dstrect);
}

inline void SDLWrapper::SDL_RenderPresent(SDL_Renderer* renderer)
{
	m_render_present(renderer);
}

inline int SDLWrapper::SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return m_render_draw_colour(renderer, r, g, b, a);
}

inline int SDLWrapper::SDL_SetWindowFullscreen(SDL_Window* window, Uint32 flags)
{
	return m_set_window_fullscreen(window, flags);
}

inline int SDLWrapper::SDL_UpdateTexture(SDL_Texture* texture, const SDL_Rect* rect, const void* pixels, int pitch)
{
	return m_update_texture(texture, rect, pixels, pitch);
}