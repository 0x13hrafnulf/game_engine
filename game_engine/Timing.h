#pragma once
#include <SDL2/SDL.h>
namespace game_engine
{
	class FpsLimiter
	{
	public:
		FpsLimiter();
		void init(float maxFPS);
		
		void setMaxFPS(float maxFPS);

		void begin();
		float end(); //return curr FPS

	private:
		float _maxFPS;
		float _fps;
		float _frameTime;

		unsigned int _startTicks;

		void calculateFPS();
	};

}