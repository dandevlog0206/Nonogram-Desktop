#pragma once

#include <forward_list>
#include "animation.h"

UI_BEGIN

class AnimationHandler;

class AnimationTrigger
{
	friend class AnimationHandler;

public:
	void cancel();

	void start();
	void stop();
	void resume();
	void reset_time();

private:
	std::vector<AnimationHandler*> handlers;
};

class AnimationHandler
{
	friend class AnimationTrigger;

public:
	AnimationHandler(AnimationTrigger* trigger);
	AnimationHandler(std::initializer_list<AnimationTrigger*> triggers);

	void registerAnimation(std::unique_ptr<Animation>&& animation);
	void operator+=(std::unique_ptr<Animation>&& animation);

	bool empty() const;

private:
	void registerTrigger(AnimationTrigger* trigger);

private:
	std::forward_list<std::unique_ptr<Animation>> animations;
	std::forward_list<AnimationTrigger*>          triggers;
};

UI_END