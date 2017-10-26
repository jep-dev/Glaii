#include "geometry.hpp"
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::setw;
using std::setprecision;

using namespace Geometry;

std::ostream& table(std::ostream& dest) {
	// Quat_t<float> x[] = {{0,0,0,1}, {1,0,0,0}, {0,1,0,0}, {0,0,1,0}};
	Quat_t<float> x[] = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
	const std::string sep_str = " | ";
	const unsigned n = 4, w = 6, sep = sep_str.size();
	for(unsigned i = 0; i <= n; i++) {
		std::ostringstream oss;
		if(i) {
			oss << x[i-1];
			dest << ' ' << std::setw(w-2) << oss.str() << ' ';
		} else {
			dest << std::setw(w + sep) << "";
		}
	}
	dest << '\n' << std::setw(w+sep-1) << ""
			<< std::setw(w*4+1) << std::setfill('_')
			<< "" << std::setfill(' ') << std::endl;
	for(unsigned i = 0; i < n; i++) {
		{
			std::ostringstream oss;
			oss << x[i];
			dest << std::setw(w) << oss.str() << sep_str;
		}
		for(unsigned j = 0; j < n; j++) {
			std::ostringstream oss;
			oss << x[i]*x[j];
			dest << ' ' << std::setw(w-2) << oss.str() << ' ';
		}
		endl(dest);
	}
	return dest << '\n';
}

int main(int argc, const char *argv[]) {
	using std::is_same;
	Vec_t<float> x = {1,0,0}, y = {0,1,0}, z = {0,0,1};
	Quat_t<float> ident = {0,0,0,1};
	table(cout);

	static_assert(is_same<
			decltype(rotate(M_PI, x)), Quat_t<double>
		>::value, "LCM(double, float) should be double");
	static_assert(is_same<
			decltype(rotate<float>(M_PI, x)), Quat_t<float>
		>::value, "LCM(float, float) should be float");
	static_assert(is_same<
			decltype(rotate(float(M_PI), x)), Quat_t<float>
		>::value, "LCM(float(double), float) should be float");

	std::map<const char*, Quat_t<float>> xforms = {
		{"pi/2 . e_i", rotate<float>(M_PI/2, x)},
		{"pi/2 . e_j", rotate<float>(M_PI/2, y)},
		{"pi/2 . e_k", rotate<float>(M_PI/2, z)}
	};
	for(auto const& kv : xforms) {
		auto const& label = kv.first;
		auto const& q = kv.second;
		cout << "Rotation Q = " << label << ":\n  q   = " << q
			<< "\n  q*  = " << *q << "\n  qq* = " << (q**q) << endl;
		for(auto const& p : { x, y, z }) {
			cout << "    p = " << p << " --> qpq* = " << q*p << endl;
		}
	}
}
