#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SDL.h>
#include <SDL_stdinc.h>
#include "../sdlwrapper.h"


class MockSDL : public SDLWrapper
{
public:
	MOCK_METHOD(SDL_Renderer*, SDL_CreateRenderer, (SDL_Window* window, int index, Uint32 flags), ());
	MOCK_METHOD(SDL_Texture*, SDL_CreateTexture, (SDL_Renderer* renderer, Uint32 format, int access, int w, int h), ());
	MOCK_METHOD(SDL_Window*, SDL_CreateWindow, (const char* title, int x, int y, int w, int h, Uint32 flags), ());
	MOCK_METHOD(void, SDL_DestroyRenderer, (SDL_Renderer* renderer), ());
	MOCK_METHOD(void, SDL_DestroyWindow, (SDL_Window* window), ());
	MOCK_METHOD(int, SDL_GetCurrentDisplayMode, (int index, SDL_DisplayMode* display_mode), ());
	MOCK_METHOD(int, SDL_Init, (Uint32 flags), ());
	MOCK_METHOD(int, SDL_PollEvent, (SDL_Event* event), ());
	MOCK_METHOD(void, SDL_Quit, (), ());
	MOCK_METHOD(int, SDL_RenderClear, (SDL_Renderer* renderer), ());
	MOCK_METHOD(int, SDL_RenderCopy, (SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect), ());
	MOCK_METHOD(void, SDL_RenderPresent, (SDL_Renderer* renderer), ());
	MOCK_METHOD(int, SDL_SetRenderDrawColor, (SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a), ());
	MOCK_METHOD(int, SDL_SetWindowFullscreen, (SDL_Window* window, Uint32 flags), ());
	MOCK_METHOD(int, SDL_UpdateTexture, (SDL_Texture* texture, const SDL_Rect* rect, const void* pixels, int pitch), ());
};