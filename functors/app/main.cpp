#include "compose.hpp"
#include "handler.hpp"
#include "events.hpp"

#include <iostream>
#include <iomanip>
#include <ostream>
#include <string>
#include <sstream>

#include <algorithm>
#include <math.h>
#include <numeric>
#include <vector>

template<typename...> struct make_void
	{ typedef void type; };
template<typename... T> using void_t =
	typename make_void<T...>::type;



/*
template<typename T, typename = void_t<>>
struct has_type_member: std::false_type {};
template<typename T>
struct has_type_member<T,
	void_t<typename T::type>>: std::true_type {};
*/

template<typename T, typename VT = void_t<>>
struct get_type {
	friend std::ostream& operator<<(std::ostream& os,
		get_type<T, VT> const& t) {
		return os << "(none)", os;
	}
};
template<typename T>
struct get_type<T,
	void_t<typename T::type>> {
	friend std::ostream& operator<<(std::ostream& os,
			get_type<T, void_t<typename T::type>> const& t) {
		return os << typename T::type {}, os;
	}
};

struct Base {
	const char *name = "Base";
};
struct Base1: Base {
	const char *name = "Base1";
};
struct Base2: Base {
	const char *name = "Base2";
};
struct Derived: Base1 {
	const char *name = "Derived";
};
template<typename S, typename T>
auto operator<<(S && os, T && t)
-> std::enable_if_t<std::is_base_of<Base,T>::value,S> {
	return os << t.name, std::forward<S>(os);
}


int main(int argc, const char *argv[]) {
	using std::cout;
	using std::endl;
	using std::common_type;

	cout << "Common(" << Base{} << ", " << Base1{} << ") = "
		<< get_type<common_type<Base, Base1>> {} << endl;
	cout << "Common(" << Base1{} << ", " << Base2{} << ") = "
		<< get_type<common_type<Base1, Base2>> {} << endl;
	cout << "Common(" << Base1{} << ", " << Derived{} << ") = "
		<< get_type<common_type<Base1, Derived>> {} << endl;
	cout << "Common(" << Base{} << ", " << Derived{} << ") = "
		<< get_type<common_type<Base, Derived>> {} << endl;
	cout << "Common(" << Derived{} << ", " << Derived{} << ") = "
		<< get_type<common_type<Derived, Derived>> {} << endl;
}
