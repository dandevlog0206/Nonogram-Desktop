#pragma once

#include "animation.h"

UI_BEGIN

class AnimationGroup;

class AnimationGroupConstIterator {
	using vector_type     = std::vector<std::unique_ptr<Animation>>;
	using vector_iterator = vector_type::const_iterator;

public:
	using iterator_category = std::forward_iterator_tag;
	using value_type        = const Animation&;
	using pointer           = const Animation*;
	using reference         = const Animation&;

	AnimationGroupConstIterator() noexcept : iter() {}

	AnimationGroupConstIterator(vector_iterator iter) noexcept 
		: iter(iter)
	{}

	_NODISCARD inline reference operator*() const noexcept {
		return **iter;
	}

	_NODISCARD inline pointer operator->() const noexcept {
		return iter->get();
	}

	inline AnimationGroupConstIterator& operator++() noexcept {
		++iter;
		return *this;
	}

	inline AnimationGroupConstIterator operator++(int) noexcept {
		AnimationGroupConstIterator temp = *this;
		++*this;
		return temp;
	}

	_NODISCARD inline bool operator==(const AnimationGroupConstIterator& _Right) const noexcept {
		return iter == _Right.iter;
	}

	_NODISCARD bool operator!=(const AnimationGroupConstIterator& _Right) const noexcept {
		return !(*this == _Right);
	}

	_NODISCARD bool operator<(const AnimationGroupConstIterator& _Right) const noexcept {
		return iter < _Right.iter;
	}

	_NODISCARD bool operator>(const AnimationGroupConstIterator& _Right) const noexcept {
		return _Right < *this;
	}

	_NODISCARD bool operator<=(const AnimationGroupConstIterator& _Right) const noexcept {
		return !(_Right < *this);
	}

	_NODISCARD bool operator>=(const AnimationGroupConstIterator& _Right) const noexcept {
		return !(*this < _Right);
	}

protected:
	vector_iterator iter;
};

class AnimationGroupIterator : public AnimationGroupConstIterator {
	using vector_type     = std::vector<std::unique_ptr<Animation>>;
	using vector_iterator = vector_type::const_iterator;

public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type        = Animation&;
	using difference_type   = vector_type::difference_type;
	using pointer           = Animation*;
	using reference         = Animation&;

	using AnimationGroupConstIterator::AnimationGroupConstIterator;

	_NODISCARD inline reference operator*() const noexcept {
		return **iter;
	}

	_NODISCARD inline pointer operator->() const noexcept {
		return iter->get();
	}

	inline AnimationGroupIterator& operator++() noexcept {
		AnimationGroupConstIterator::operator++();
		return *this;
	}

	inline AnimationGroupIterator operator++(int) noexcept {
		AnimationGroupIterator temp = *this;
		AnimationGroupConstIterator::operator++();
		return temp;
	}
};

class AnimationGroup : Animation
{
public:
	using value_type     = std::unique_ptr<Animation>;
	using iterator       = AnimationGroupIterator;
	using const_iterator = AnimationGroupConstIterator;

	void apend(std::unique_ptr<Animation>&& animation);

	void update(float dt) override;
	void cancel() override;

	void start() override;
	void stop() override;
	void resume() override;
	void reset_time() override;

	size_t size() const;

	void clear();

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	Animation& operator[](size_t idx);
	const Animation& operator[](size_t idx) const;

private:
	std::vector<value_type> arr;
};

UI_END