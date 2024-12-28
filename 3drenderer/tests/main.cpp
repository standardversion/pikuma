#include <SDL.h>
#include "gtest/gtest.h"

#define SDL_MAIN_HANDLED

int main(int argc, char** argv)
{
	// Also needed for my dual main problem
	SDL_SetMainReady();

	// Initializing SDL (with no audio because it will not work on Travis)
	// I have some tests working with SDL surfaces and textures, so I need this subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "FATAL: Cannot init SDL: " << SDL_GetError();
		return -1;
	}

	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	SDL_Quit();  // Quit SDL properly

	return result;
}