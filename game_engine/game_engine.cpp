#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "game_engine.h"



namespace game_engine {

	int init()
	{
		//Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


		return 0;
	}


}

