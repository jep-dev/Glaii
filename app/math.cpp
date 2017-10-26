#include "geometry.hpp"
#include "matrix.hpp"

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
	using OSS = std::ostringstream;
	using std::setw;
	using std::setfill;
	using std::endl;

	const std::string sep_str = " | ";
	const unsigned n = 4, w = 6, sep = sep_str.size();
	Quat_t<float> x[] = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};

	dest << setw(w+sep-1) << "";

	dest << (sep ? '_' : ' ');
	for(auto i = 0; i < n; i++) {
		OSS oss;
		oss << x[i];
		dest << '_' << setfill('_') << setw(w-2) << oss.str() << '_';
		dest << setfill(' ');
	}
	dest << '\n';
	for(auto i = 0; i < n; i++) {
		{
			OSS oss;
			oss << x[i];
			dest << setw(w) << oss.str() << sep_str;
		}
		for(unsigned j = 0; j < n; j++) {
			OSS oss;
			oss << x[i]*x[j];
			dest << ' ' << setw(w-2) << oss.str() << ' ';
		}
		dest << '\n';
	}
	return endl(dest);
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
