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
	xyzw<float> x[] = {{0,0,0,1}, {1,0,0,0}, {0,1,0,0}, {0,0,1,0}};
	unsigned n = 0;
	for(unsigned i = 0; i < 4; i++) {
		for(unsigned j = 0; j < 4; j++) {
			std::ostringstream oss;
			oss << x[i]*x[j];
			dest << std::setw(4) << oss.str() << ' ';
			n++;
		}
		endl(dest);
	}
	return dest;
}

int main(int argc, const char *argv[]) {
	typedef xyzw<float> quat;
	typedef xyz<float> vert;
	vert x = {1,0,0}, y = {0,1,0}, z = {0,0,1};
	quat ident = {0,0,0,1};
	table(cout);

	std::map<const char*, quat> xforms = {
		{"pi/2 . e_i", rotate(float(M_PI/2), x)},
		{"pi/2 . e_j", rotate(float(M_PI/2), y)},
		{"pi/2 . e_k", rotate(float(M_PI/2), z)}
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
