#include "events.hpp"

#include <ostream>
// #include <sstream>

std::ostream& operator<<(std::ostream &dest, CommonEvent const& ev) {
	dest << "{" << ev.data1 << ", " << ev.data2 << "}";
}
std::ostream& operator<<(std::ostream &dest, Event const& ev) {
	dest << "Event ";
	return dest;
}
