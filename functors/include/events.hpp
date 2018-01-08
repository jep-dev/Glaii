#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <iosfwd>
#include <utility>
// #include <string>

typedef enum {
	MouseEventType = 0,
	KeyboardEventType,
	QuitEventType
} EventType;

struct CommonEvent {
	const EventType type;
	unsigned data1, data2;

	template<typename S>
	friend S && operator<<(S &&, CommonEvent const&);

	CommonEvent(EventType && type, unsigned d1 = 0, unsigned d2 = 0):
		type(type), data1(d1), data2(d2) {}
};
std::ostream& operator<<(std::ostream& dest, CommonEvent const& ev);

struct MouseEvent : CommonEvent {
	template<typename... T> MouseEvent(T &&... t):
		CommonEvent(MouseEventType, std::forward<T>(t)...) {}
};

struct KeyboardEvent : CommonEvent {
	template<typename... T> KeyboardEvent(T &&... t):
		CommonEvent(KeyboardEventType, std::forward<T>(t)...) {}
};

struct QuitEvent : CommonEvent {
	template<typename... T> QuitEvent(T &&... t):
		CommonEvent(QuitEventType, std::forward<T>(t)...) {}
};


struct Event {

	EventType type;
	MouseEvent evMouse;
	KeyboardEvent evKeyboard;
	QuitEvent evQuit;

	friend std::ostream&
	operator<<(std::ostream& dest, Event const& ev);
};

template<typename T>
auto pretty_function(T const& t)
	{ return __PRETTY_FUNCTION__; }
template<>
auto pretty_function(CommonEvent const& ce)
	{ return "CommonEvent"; }
template<>
auto pretty_function(MouseEvent const& ce)
	{ return "MouseEvent"; }
template<>
auto pretty_function(KeyboardEvent const& ce)
	{ return "KeyboardEvent"; }
template<>
auto pretty_function(QuitEvent const& ce)
	{ return "QuitEvent"; }
template<>
auto pretty_function(Event const& ce)
	{ return "Event"; }


#endif
