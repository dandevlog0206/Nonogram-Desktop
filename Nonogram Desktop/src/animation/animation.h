#pragma once

#include <vector>
#include <memory>
#include <vkdl/util/delegate.hpp>
#include <vkdl/util/property.hpp>
#include <vkdl/math/vector_type.h>
#include "../config.h"

UI_BEGIN

class Animation abstract
{
	PROPERTY_INIT(Animation);

public:
	Animation();

	virtual void update(float dt) = 0;
	virtual void cancel() = 0;

	virtual void start() = 0;
	virtual void stop();
	virtual void resume();
	virtual void reset_time();

	PROPERTY {
		PROPERTY_DEFAULT_GET_SET(float, TimeOffset);
		PROPERTY_DEFAULT_GET_SET(float, Speed);
	};

	bool is_running() const;

protected:
	float time;
	bool  running;
};

template <class T>
class GenericAnimation abstract : public Animation
{
public:
	GenericAnimation() :
		prev_val()
	{}

	void registerVariable(T* var)
	{
		getter = [=]() { return *var; };
		setter = [=](float value) { *var = value; };
	}

	void registerGetter(const Delegate<T(void)>& get)
	{
		getter = get;
	}

	void registerSetter(const Delegate<void(T)>& set)
	{
		setter = set;
	}

	void update(float dt) override
	{
		if (!running) return;

		T new_val = eval(Speed * (time += dt) + TimeOffset);
		if (setter) setter(getter() + new_val - std::exchange(prev_val, new_val));
	}

	void cancel() override
	{
		running = false;
		time    = 0.f;
		if (setter) setter(getter() - std::exchange(prev_val, {}));
	}

	void start() override
	{
		if (running) return;

		running = true;
		time    = 0.f;
		if (setter) setter(getter() - std::exchange(prev_val, {}));
	}

	T value() const
	{
		return eval(time);
	}
	
	void reset()
	{
		getter.clear();
		setter.clear();
		prev_val = 0.f;
		time     = 0.f;
		running  = false;
	}

protected:
	virtual T eval(float t) const = 0;

private:
	Delegate<T(void)> getter;
	Delegate<void(T)> setter;
	T prev_val;
};

class FadingAnimation : public GenericAnimation<float>
{
	PROPERTY_INIT(FadingAnimation);

public:
	FadingAnimation();

	enum FadeType {
		Decrease,
		Increase
	};

	PROPERTY{
		PROPERTY_DEFAULT_GET_SET(FadeType, FadeEffect);
		PROPERTY_DEFAULT_GET_SET(float, TotalTime);
		PROPERTY_DEFAULT_GET_SET(float, MinValue);
		PROPERTY_DEFAULT_GET_SET(float, MaxValue);
	};

private:
	float eval(float t) const override;
};

class FloatingAnimation : public GenericAnimation<float>
{
	PROPERTY_INIT(FloatingAnimation);

public:
	PROPERTY {
		PROPERTY_DEFAULT_GET_SET(float, Amplitude);
	};

private:
	float eval(float t) const override;
};

class TransitionAnimation : public GenericAnimation<vkdl::vec2>
{
	PROPERTY_INIT(TransitionAnimation);

public:
	void update(float dt) override;

	float eval_play_time() const;

	PROPERTY {
		PROPERTY_DEFAULT_GET_SET(float, Distance);
		PROPERTY_DEFAULT_GET_SET(float, Acceleration);
		PROPERTY_DEFAULT_GET_SET(vkdl::vec2, DirectionVector);
	};

private:
	vkdl::vec2 eval(float t) const override;
};

UI_END