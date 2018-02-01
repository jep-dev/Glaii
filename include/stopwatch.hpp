#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

/** @file stopwatch.hpp
 *  @brief Defines types to record and analyze durations. */

///@cond
#include <limits>
#include <utility>
#include <deque>
#include <SDL_timer.h>
///@endcond


long perf_ticks(void) { return SDL_GetPerformanceCounter(); }
long perf_freq(void) { return SDL_GetPerformanceFrequency(); }

template<typename T = float>
T perf_rate(void) { return perf_ticks() / T(perf_freq()); }

struct Sample {
	float (*measure)(void);

	operator float(void) const;
	auto start(void) -> decltype(*this);
	auto pause(void) -> decltype(*this);
	auto stop(void) -> decltype(*this);
	Sample(float (*measure)(void));
protected:
	float origin = 0, delta = 0;
	bool running = false, started = false;
};

struct Stopwatch {
	float (*measure)(void);
	unsigned max_samples;

	auto start(void) -> decltype(*this);
	float pause(void);
	float average(bool overall = false) const;
	float deviation(void) const;

	template<typename OS>
	friend OS& operator<<(OS& os, Stopwatch const& s) {
		bool any = false;
		float recent = s.average(false), overall = s.average(true),
			 dev = s.deviation(), scale = perf_freq();
		auto size = s.samples.size();
		if(s.index) os << "Average FPS: " << recent
			<< " (deviation " << dev << ")";
		return os;
	}

	Stopwatch(float (*measure)(void), unsigned max_samples = 1);
protected:
	unsigned index = 0, counter = 0;
	float slice = 0, total = 0;
	std::deque<float> samples;
	Sample sample;
};

template<typename R, typename... T> auto stopwatch(R && r, T &&... t) {
	return Stopwatch(std::forward<R>(r), std::forward<T>(t)...);
}

#endif
