#pragma once

#include <chrono>
#include <assert.h>

#define TIME_IMPLEMENTATION Time* Time::instance = nullptr;

class Time
{
public:
	inline Time() :
		running(std::chrono::steady_clock::now()),
		last(std::chrono::steady_clock::now()),
		total_elapsed(0.f),
		delta(0),
		target_fps(60)
	{
		assert(instance == nullptr);
		instance = this;
	}
	
	static inline float fps()
	{
		return 1.f / instance->delta;
	}

	static inline float elapsed()
	{
		return instance->total_elapsed;
	}

	static inline float dt()
	{
		return instance->delta;
	}

	inline void newFrame()
	{
		auto now = std::chrono::steady_clock::now();
		delta         = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() * 1e-6f;
		total_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - running).count() * 1e-6f;
		last  = now;
	}

	inline void setFps(float fps)
	{
		target_fps = fps;
	}

	inline void waitForFPS()
	{

	}

private:
	static Time* instance;
	
	std::chrono::steady_clock::time_point running;
	std::chrono::steady_clock::time_point last;
	float total_elapsed;
	float delta;
	float target_fps;
};
