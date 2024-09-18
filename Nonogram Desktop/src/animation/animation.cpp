#include "animation.h"

#include <math.h>

using namespace vkdl;

UI_BEGIN

Animation::Animation() :
	TimeOffset(0.f),
	Speed(1.f),
	time(0.f),
	running(false)
{
}

void Animation::stop()
{
	running = false;
}

void Animation::resume()
{
	running = true;
}

void Animation::reset_time()
{
	time = 0.f;
}

bool Animation::is_running() const
{
	return running;
}


FadingAnimation::FadingAnimation() :
	FadeEffect(FadeType::Decrease),
	TotalTime(1.f),
	MinValue(0.f),
	MaxValue(1.f)
{}

float FadingAnimation::eval(float t) const
{
	if (TotalTime <= t)
		return FadeEffect == Increase ? MaxValue : MinValue;

	float slope = ((float)MaxValue - MinValue) / TotalTime;

	if (FadeEffect == Increase)
		return slope * t + MinValue;
	else
		return -slope * t + MaxValue;
}

float FloatingAnimation::eval(float t) const
{
	return Amplitude * sinf(t);
}

void TransitionAnimation::update(float dt)
{
	GenericAnimation<vec2>::update(dt);

	if (eval_play_time() < time) {
		stop();
		reset_time();
	}
}

float TransitionAnimation::eval_play_time() const
{
	return 2.f * sqrtf((float)Distance / Acceleration);
}

vec2 TransitionAnimation::eval(float t) const
{
	vec2 dir        = glm::normalize((vec2)DirectionVector);
	float dist      = Distance.get();
	float a         = Acceleration.get();
	float half_time = sqrtf(dist / a);
	
	if (t < half_time) {
		return dir * a * 0.5f * t * t;
	} else if (half_time <= t && t < 2.f * half_time) {
		return dir * (-0.5f * a * t * t + 2.f * a * half_time * t - dist);
	} else {
		return dist * dir;
	}
}

UI_END