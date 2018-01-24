#ifndef H_UTILITIES
#define H_UTILITIES

// Timer Class
#pragma region Timer
enum TimerState { RUNNING, STOPPED, PAUSED };

class Timer {
private:
	int mStartTicks;
	int mPausedTicks;
	TimerState mState;
public:
	Timer();
	TimerState getState() { return mState; };
	int getTicks();
	void pause();
	void unpause();
	void start();
	void stop();
};
#pragma endregion

// Color Class
#pragma region Color
class Color {
public:
	int r;
	int g;
	int b;
	Color(int red, int green, int blue);
};
#pragma endregion

#else
#pragma message("utilities.h superfluously included")
#endif //H_UTILITIES