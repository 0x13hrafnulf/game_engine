#include "window.h"
#include "errors.h"
namespace game_engine
{

	window::window()
	{
	}


	window::~window()
	{
	}

	int window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlag)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		if (currentFlag & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlag & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlag & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}


		_sdlwindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
		if (_sdlwindow == nullptr)
		{
			fatalError("SDL Window cannot be created!");
		}

		SDL_GLContext glcontext = SDL_GL_CreateContext(_sdlwindow);
		if (glcontext == nullptr)
		{
			fatalError("SDL Window cannot be created!");
		}

		GLenum gl_error = glewInit();
		if (gl_error != GLEW_OK)
		{
			fatalError("GLEW cannot be initialized!");
		}

		std::printf("*** OpenGL  Version: %s ***", glGetString(GL_VERSION));

		glClearColor(0.0f, 0.0f, 1.0f, 1.0);

		SDL_GL_SetSwapInterval(1); //turn on vsync

		//enable blending, alpha = 0

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return 0;
	}

	void window::swapBuffer()
	{

		SDL_GL_SwapWindow(_sdlwindow);

	}
}