#include "models.hpp"
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

using namespace Models;

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

	quat xforms[] = {
		quat{float(sin(M_PI/8)), 0, 0, float(cos(M_PI/8))},
		rotate(float(M_PI/4), x)
	};
	std::cout << "xforms:\n [0] = " << xforms[0]
		<< "\n [1] = " << xforms[1] << endl;
	std::cout << " [1] - [0] = " << xforms[1]-xforms[0]
		<< ";\n |[1]-[0]| ~= 0? " << std::boolalpha
		<< near(xforms[0], xforms[1]) << endl;

	/*for(auto && q :) {
		cout << q << endl;
		cout << "Natural: " << q << endl;
		cout << "  [i]=" << q['i'] << "  [j]=" << q['j']
			<< "  [k]=" << q['k'] << "  [1]=" << q['1'] << endl;
		cout << "Near zero? " << std::boolalpha << near_zero(q['i'])
			<< ", " << std::boolalpha << near_zero(q['j'])
			<< ", " << std::boolalpha << near_zero(q['k'])
			<< ", " << std::boolalpha << near_zero(q['1']) << endl;
	}*/
}
