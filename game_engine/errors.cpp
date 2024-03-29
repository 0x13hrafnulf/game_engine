#include "errors.h"

#include <cstdlib>
#include <iostream>
#include <SDL2\SDL.h>


namespace game_engine {

	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(1);
	}


}