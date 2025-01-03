#pragma once
#include <SDL.h>
#include <SDL_stdinc.h>

// Wrapper class for SDL so we can mock it for tests
class SDLWrapper
{
public:
	virtual ~SDLWrapper() = default;

	virtual SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags)
	{
		//namespace resolution required otherwise this will recurse
		return ::SDL_CreateRenderer(window, index, flags);
	}

	virtual SDL_Texture* SDL_CreateTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h)
	{
		return ::SDL_CreateTexture(renderer, format, access, w, h);
	}

	virtual SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
	{
		return ::SDL_CreateWindow(title, x, y, w, h, flags);
	}

	virtual void SDL_Delay(Uint32 ms)
	{
		::SDL_Delay(ms);
	}

	virtual void SDL_DestroyRenderer(SDL_Renderer* renderer)
	{
		::SDL_DestroyRenderer(renderer);
	}

	virtual void SDL_DestroyWindow(SDL_Window* window)
	{
		::SDL_DestroyWindow(window);
	}

	virtual int SDL_GetCurrentDisplayMode(int index, SDL_DisplayMode* display_mode)
	{
		return ::SDL_GetCurrentDisplayMode(index, display_mode);
	}

	virtual Uint32 SDL_GetTicks()
	{
		return ::SDL_GetTicks();
	}

	virtual int SDL_Init(Uint32 flags)
	{
		return ::SDL_Init(flags);
	}

	virtual int SDL_PollEvent(SDL_Event* event)
	{
		return ::SDL_PollEvent(event);
	}

	virtual void SDL_Quit()
	{
		::SDL_Quit();
	}

	virtual int SDL_RenderClear(SDL_Renderer* renderer)
	{
		return ::SDL_RenderClear(renderer);
	}

	virtual int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
	{
		return ::SDL_RenderCopy(renderer, texture, srcrect, dstrect);
	}

	virtual void SDL_RenderPresent(SDL_Renderer* renderer)
	{
		::SDL_RenderPresent(renderer);
	}

	virtual int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		return ::SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	virtual int SDL_SetWindowFullscreen(SDL_Window* window, Uint32 flags)
	{
		return ::SDL_SetWindowFullscreen(window, flags);
	}

	virtual int SDL_UpdateTexture(SDL_Texture* texture, const SDL_Rect* rect, const void* pixels, int pitch)
	{
		return ::SDL_UpdateTexture(texture, rect, pixels, pitch);
	}
};