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

/** @brief Shorthand for SDL's performance counter, which increases
 * monotonically and provides an accurate measurement of duration combined
 * with perf_freq. */
long perf_ticks(void) { return SDL_GetPerformanceCounter(); }
/** @brief Shorthand for SDL's performance frequency, which provides the
 * ratio between counter ticks and real time. */
long perf_freq(void) { return SDL_GetPerformanceFrequency(); }

/** @brief Performance metric; useful only when subtracted from another call
 * to the same function. Note that each measurement is scaled by the current
 * frequency as opposed to two counter checks with one frequency check. The
 * most accurate approach to duration measurement might be to integrate
 * smaller intervals. */
template<typename T = float>
T perf_rate(void) { return perf_ticks() / T(perf_freq()); }

/** @brief A type used to measure durations with simple controls. */
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

/** @brief A type used to buffer measurements taken from the Sample type and
 * provide an average and standard deviation of frames/events per second
 * based on its contents. */
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

/** @brief Creates a stopwatch from the given function, which may not have
 * the same signature as (but must be convertible to) the function used by
 * Sample to take measurements of the current performance rate. */
template<typename R, typename... T> auto stopwatch(R && r, T &&... t) {
	return Stopwatch(std::forward<R>(r), std::forward<T>(t)...);
}

#endif
