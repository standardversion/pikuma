#include <iostream>
#include <SDL.h>
#include <array>
#include <cstdint>
#include <memory>

//https://wiki.libsdl.org/SDL2/CategoryAPI

bool initialize_window(SDL_Window*& window, SDL_Renderer*& renderer, const int width, const int height)
{
	// Initialize the SDL library
	// SDL_INIT_EVERYTHING flag initializes audio, video, controller etc subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Error initializing SLD.\n";
		return false;
	}
	// Create the SDL Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{
		std::cout << "Error creating SDL window.\n";
		return false;
	}
	// Create SDL Renderer, a 2d rendering context for a window
	// -1 to initialize the first driver supporting the requested flags.
	// 0 is no flags requested
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		std::cout << "Error creating SDL renderer.\n";
		return false;
	}
	return true;
}


bool setup(
	SDL_Texture*& colour_buffer_texture,
	SDL_Window*& window,
	SDL_Renderer*& renderer,
	const int width,
	const int height
) {
	// first create the window and rendering context
	bool is_running = initialize_window(window, renderer, width, height);
	// Create a texture for a rendering context. ie to display the colour buffer
	colour_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width,
		height
	);
	
	return is_running;
}


void process_input(bool& is_running)
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
		break;
	}
}

void update()
{

}

void clear_colour_buffer(std::uint32_t*& colour_buffer, const int width, const int height, std::uint32_t colour)
{
	// our 2d pixels (x * y) are laid out in a 1d array
	// so for example, if we're dealing with 1920x1080 the first 1920 indices for the array
	// will contain row 1 of the pixels on the screen 1921 to 3840 will contain row 2 ...
	// so we start with the height ie row one then loop across  with width horizontally to fill that row
	// with colour values
	// to access any specific pixel we can get the index using [(width * row) + column]
	for (int y{ 0 }; y < height; y++)
	{
		for (int x{ 0 }; x < width; x++)
		{
			colour_buffer[(width * y) + x] = colour;
		}
	}
}

void render_colour_buffer(SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, const int width, SDL_Renderer*& renderer)
{
	// update the texture with the contents of the colour buffer
	SDL_UpdateTexture(
		colour_buffer_texture,
		NULL,
		colour_buffer,
		static_cast<int>(width * sizeof(std::uint32_t))
	);
	// copy the texture onto the renderer, null, null copies entire texture
	SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL);
}

void render(SDL_Renderer*& renderer, SDL_Texture*& colour_buffer_texture, std::uint32_t*& colour_buffer, const int width, const int height)
{
	// set the renderer colour to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// clear the renderer
	SDL_RenderClear(renderer);
	// render the colour buffer
	render_colour_buffer(colour_buffer_texture, colour_buffer, width, renderer);
	// fill the colour buffer with a colour value
	clear_colour_buffer(colour_buffer, width, height, 0xFFFFFF00);
	// Update the screen with any rendering performed since the previous call.
	SDL_RenderPresent(renderer);
}

void cleanup(SDL_Window*& window, SDL_Renderer*& renderer, std::uint32_t*& colour_buffer)
{
	// SDL cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	// set pointers to nullptr
	renderer = nullptr;
	window = nullptr;
	// deallocate the memory
	delete[] colour_buffer;
	colour_buffer = nullptr;
}

int main(int argc, char* argv[])
{
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* colour_buffer_texture{ nullptr };
	constexpr const int width{ 1920 };
	constexpr const int height{ 1080 };
	std::uint32_t* colour_buffer{ new std::uint32_t[width * height]{} };
	bool is_running{ setup(colour_buffer_texture, window, renderer, width, height) };
	while (is_running)
	{
		process_input(is_running);
		update();
		render(renderer, colour_buffer_texture, colour_buffer, width, height);
	}
	cleanup(window, renderer, colour_buffer);
	return 0;
}