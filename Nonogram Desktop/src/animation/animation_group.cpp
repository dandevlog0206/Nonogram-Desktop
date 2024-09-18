#include "animation_group.h"

UI_BEGIN

void AnimationGroup::cancel()
{
	for (const auto& animation : arr)
		animation->cancel();
}

void AnimationGroup::apend(std::unique_ptr<Animation>&& animation)
{
	arr.emplace_back(std::move(animation));
}

void AnimationGroup::update(float dt)
{
	for (const auto& animation : arr)
		animation->update(dt);
}

void AnimationGroup::start()
{
	for (const auto& animation : arr)
		animation->start();
}

void AnimationGroup::stop()
{
	for (const auto& animation : arr)
		animation->stop();
}

void AnimationGroup::resume()
{
	for (const auto& animation : arr)
		animation->resume();
}

void AnimationGroup::reset_time()
{
	for (const auto& animation : arr)
		animation->reset_time();
}

size_t AnimationGroup::size() const
{
	return arr.size();
}

void AnimationGroup::clear()
{
	arr.clear();
}

AnimationGroup::iterator AnimationGroup::begin()
{
	return arr.begin();
}

AnimationGroup::iterator AnimationGroup::end()
{
	return arr.end();
}

AnimationGroup::const_iterator AnimationGroup::begin() const
{
	return arr.begin();
}

AnimationGroup::const_iterator AnimationGroup::end() const
{
	return arr.end();
}

Animation& AnimationGroup::operator[](size_t idx)
{
	return *arr[idx];
}

const Animation& AnimationGroup::operator[](size_t idx) const
{
	return *arr[idx];
}

UI_END