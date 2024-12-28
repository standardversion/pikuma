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
using SetWindowFullscreen_Fcn = std::function<int(SDL_Window*, Uint32)>;
using UpdateTexture_Fcn = std::function<int(SDL_Texture*, const SDL_Rect*, const void*, int)>;

// SDL Implimentation
class SDLImpl
{
public:
	CreateRenderer_Fcn SDL_CreateRenderer{};
	CreateTexture_Fcn SDL_CreateTexture{};
	CreateWindow_Fcn SDL_CreateWindow{};
	DestroyRenderer_Fcn SDL_DestroyRenderer{};
	DestroyWindow_Fcn SDL_DestroyWindow{};
	GetCurrentDisplayMode_Fcn SDL_GetCurrentDisplayMode{};
	Init_Fcn SDL_Init{};
	PollEvent_Fcn SDL_PollEvent{};
	Quit_Fcn SDL_Quit{};
	RenderClear_Fcn SDL_RenderClear{};
	RenderCopy_Fcn SDL_RenderCopy{};
	RenderPresent_Fcn SDL_RenderPresent{};
	SetRenderDrawColour_Fcn SDL_SetRenderDrawColor{};
	SetWindowFullscreen_Fcn SDL_SetWindowFullscreen{};
	UpdateTexture_Fcn SDL_UpdateTexture{ };
};

class SDLWrapper
{
public:
	SDLWrapper(SDLImpl sdl)
		: m_sdl{sdl}
	{
	}

	SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags)
	{
		return m_sdl.SDL_CreateRenderer(window, index, flags);
	}

	SDL_Texture* SDL_CreateTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h)
	{
		return m_sdl.SDL_CreateTexture(renderer, format, access, w, h);
	}

	SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
	{
		return m_sdl.SDL_CreateWindow(title, x, y, w, h, flags);
	}

	void SDL_DestroyRenderer(SDL_Renderer* renderer)
	{
		m_sdl.SDL_DestroyRenderer(renderer);
	}

	void SDL_DestroyWindow(SDL_Window* window)
	{
		m_sdl.SDL_DestroyWindow(window);
	}

	int SDL_GetCurrentDisplayMode(int index, SDL_DisplayMode* display_mode)
	{
		return m_sdl.SDL_GetCurrentDisplayMode(index, display_mode);
	}

	virtual int SDL_Init(Uint32 flags)
	{
		return m_sdl.SDL_Init(flags);
	}

	int SDL_PollEvent(SDL_Event* event)
	{
		return m_sdl.SDL_PollEvent(event);
	}

	void SDL_Quit()
	{
		m_sdl.SDL_Quit();
	}

	int SDL_RenderClear(SDL_Renderer* renderer)
	{
		return m_sdl.SDL_RenderClear(renderer);
	}

	int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
	{
		return m_sdl.SDL_RenderCopy(renderer, texture, srcrect, dstrect);
	}

	void SDL_RenderPresent(SDL_Renderer* renderer)
	{
		m_sdl.SDL_RenderPresent(renderer);
	}

	int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		return m_sdl.SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	int SDL_SetWindowFullscreen(SDL_Window* window, Uint32 flags)
	{
		return m_sdl.SDL_SetWindowFullscreen(window, flags);
	}

	int SDL_UpdateTexture(SDL_Texture* texture, const SDL_Rect* rect, const void* pixels, int pitch)
	{
		return m_sdl.SDL_UpdateTexture(texture, rect, pixels, pitch);
	}
private:
	SDLImpl m_sdl{};
};
