#include "Timing.h"

namespace game_engine
{
	FpsLimiter::FpsLimiter()
	{

	}
	void FpsLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}
	void FpsLimiter::setMaxFPS(float maxFPS)
	{
		_maxFPS = maxFPS;
	}
	void FpsLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}
	float FpsLimiter::end() //return curr FPS
	{
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		//limit the fps to the max fps
		if (1000.f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.f / _maxFPS - frameTicks);
		}

		return _fps;
	}
	void FpsLimiter::calculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currFrame = 0;

		static float prevTicks = SDL_GetTicks();
		float currTicks = SDL_GetTicks();

		_frameTime = currTicks - prevTicks;

		frameTimes[currFrame % NUM_SAMPLES] = _frameTime;

		prevTicks = currTicks;

		int count;
		currFrame++;

		if (currFrame < NUM_SAMPLES)
		{
			count = currFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;

		for (int i = 0; i < count; ++i)
		{
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
		{
			_fps = 1000.0f / frameTimeAverage;
		}
		else
		{
			_fps = 60.0f;
		}

	}
}