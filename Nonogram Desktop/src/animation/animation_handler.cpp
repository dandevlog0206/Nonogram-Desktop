#include "animation_handler.h"

UI_BEGIN

void AnimationTrigger::cancel()
{
	for (auto* handler : handlers)
		for (auto& animation : handler->animations)
			animation->cancel();
}

void AnimationTrigger::start()
{
	for (auto* handler : handlers)
		for (auto& animation : handler->animations)
			animation->start();
}

void AnimationTrigger::stop()
{
	for (auto* handler : handlers)
		for (auto& animation : handler->animations)
			animation->stop();
}

void AnimationTrigger::resume()
{
	for (auto* handler : handlers)
		for (auto& animation : handler->animations)
			animation->resume();
}

void AnimationTrigger::reset_time()
{
	for (auto* handler : handlers)
		for (auto& animation : handler->animations)
			animation->reset_time();
}

AnimationHandler::AnimationHandler(AnimationTrigger* trigger)
{
	registerTrigger(trigger);
}

AnimationHandler::AnimationHandler(std::initializer_list<AnimationTrigger*> triggers)
{
	for (auto item : triggers)
		registerTrigger(item);
}

void AnimationHandler::registerAnimation(std::unique_ptr<Animation>&& animation)
{
	animations.push_front(std::move(animation));
}

void AnimationHandler::operator+=(std::unique_ptr<Animation>&& animation)
{
	registerAnimation(std::move(animation));
}

bool AnimationHandler::empty() const
{
	return animations.empty();
}

void AnimationHandler::registerTrigger(AnimationTrigger* trigger)
{
	for (const auto* ptr : triggers)
		if (trigger == ptr)
			return;

	triggers.push_front(trigger);
	trigger->handlers.push_back(this);
}

UI_END
