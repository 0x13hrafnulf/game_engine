#pragma once


#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>

namespace game_engine
{
	enum  windowFlags
	{
		INVISIBLE = 0x1,
		FULLSCREEN = 0x2,
		BORDERLESS = 0x3
	};



	class window
	{
	public:
		window();
		~window();


		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlag);
		void swapBuffer();
		int getScreenWidth()
		{
			return _screenWidth;
		}
		int getScreenHeight()
		{
			return _screenHeight;
		}

	private:
		SDL_Window * _sdlwindow;
		int _screenWidth;
		int _screenHeight;
	};

}