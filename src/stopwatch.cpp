#include "stopwatch.hpp"

Sample::operator float(void) const {
	if(running) {
		auto update = (*measure)() - origin;
		if(update > 0) return delta + update;
	}
	return delta;
}

auto Sample::start(void) -> decltype(*this) {
	if(running) return *this;
	started = running = true;
	origin = (*measure)();
	return *this;
}

auto Sample::pause(void) -> decltype(*this) {
	if(!running) return *this;
	auto cur = (*measure)();
	if(cur > origin)
		delta += cur - origin;
	origin = cur;
	running = false;
	return *this;
}

auto Sample::stop(void) -> decltype(*this) {
	running = started = origin = delta = 0;
	return *this;
}

Sample::Sample(float (*measure)(void)): measure(measure) {}

auto Stopwatch::start(void) -> decltype(*this)
	{ sample.start(); return *this; }

float Stopwatch::pause(void) {
	float val = sample.pause();
	if(val > 0) {
		val = 1/val;
		index++;
		counter++;
		if(max_samples) {
			while(samples.size() + 1 > max_samples) {
				auto& head = samples.front();
				slice -= head;
				samples.pop_front();
			}
		}
		samples.push_back(val);
		slice += val;
		total += val;
	} else val = 0;
	sample.stop();
	return total;
}

float Stopwatch::average(bool overall) const {
	auto size = samples.size();
	return overall ? (counter ? total / float(counter) : 0)
		: (size ? slice / float(size) : 0);
}

float Stopwatch::deviation(void) const {
	auto size = samples.size();
	if(!size) return 0;
	float mean = average(), dev = 0, dev2 = 0;
	for(auto& s : samples) {
		auto diff = mean - s;
		dev += diff * diff;
	}
	dev2 = sqrt(dev/size);
	return dev2;
}

Stopwatch::Stopwatch(float (*measure)(void), unsigned max_samples):
	sample(measure) {}
