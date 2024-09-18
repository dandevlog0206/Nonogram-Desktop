#pragma once

#include <assert.h>

template <class T>
class range_iter {
	template <class U>
	friend class range_t;

	range_iter(const T& step, const T& curr) :
		step(step), curr(curr) {}

public:
	T operator*() const {
		return curr;
	}

	range_iter& operator++() {
		curr += step;
		return *this;
	}

	bool operator!=(const range_iter<T>& rhs) const {
		if (0 <= step) return curr < rhs.curr;
		else return rhs.curr < curr;
	}

private:
	T step;
	T curr;
};

template <class T>
class range_t {
	template <class U>
	friend range_t<U> range(const U&, const U&, const U&);
	template <class U>
	friend range_t<U> range(const U&);

	range_t(const T& first, const T& last, const T& step) :
		first(first), last(last), step(step) {}

public:
	range_iter<T> begin() const {
		return { step, first };
	}

	range_iter<T> end() const {
		return { step, last };
	}

private:
	T first;
	T last;
	T step;
};

template <class T>
range_t<T> range(const T& first, const T& last, const T& step = static_cast<T>(1)) {
	assert((0 <= step && first <= last) || (step < 0 && last <= first));
	return { first, last, step };
}

template <class T>
range_t<T> range(const T& last) {
	return { static_cast<T>(0), last, static_cast<T>(1) };
}