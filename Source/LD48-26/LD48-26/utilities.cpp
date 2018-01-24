#pragma region define
#define _USE_MATH_DEFINES
#pragma endregion
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
#include "freeglut.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include "utilities.h"
#include "level.h"
#include "engine.h"

// Timer Class
#pragma region Timer
Timer::Timer() {
	mStartTicks = 0;
	mPausedTicks = 0;
	mState = RUNNING;
}

void Timer::start() {
	mState = RUNNING;
	mStartTicks = SDL_GetTicks();
}

void Timer::stop() {
	mState = STOPPED;
}

void Timer::pause() {
	if(mState == RUNNING) {
		mState = PAUSED;
		mPausedTicks = SDL_GetTicks() - mStartTicks;
	}
}

void Timer::unpause() {
	if(mState == PAUSED) {
		mState = RUNNING;
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}

int Timer::getTicks() {
	if(mState == RUNNING)
		return mStartTicks;
	else if(mState == PAUSED)
		return mPausedTicks;
	return 0;
}
#pragma endregion

// Color Class
#pragma region Color
Color::Color(int red, int green, int blue) {
	r = red;
	g = green;
	b = blue;
}
#pragma endregion