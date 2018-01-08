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
//#include <utility>



int main() {
	int n = 30, trunc0 = n/4, trunc1 = n*3/4,
		ns[3] = {0, 0, 0}, ws[3] = {0, 0, 0},
		xss[n][3] = {0};

	struct {
		int handle(int x) const { return x*x+2*x+1; }
	} stateless;
	struct {
		int state = 1;
		int handle(int x) { return state += x/2; }
	} fibonacci;

	auto f = make_handler<int>(&stateless);
	auto g = make_handler<int>(&fibonacci);

	std::string delim = " | ",
			labels[3] = {"x", "f.x", "g.x"};

	std::cout << std::left;
	for(unsigned i = 0; i < n; i++) {
		int x = i/2;
		xss[i][0]=x;
		xss[i][1]=f(x);
		xss[i][2]=g(x);
		for(unsigned j = 0; j < 3; j++)
			ns[j]=std::max(xss[i][j], ns[j]);
	}
	std::cout << std::setfill('_');
	for(unsigned i = 0; i < 3; i++) {
		ws[i] = std::max(ws[i], int(labels[i].length()));
		ws[i] = std::max(ws[i],
				int(ceil(logf(ns[i])/logf(10))));
		std::cout << delim << std::setw(ws[i]) << labels[i];
	}
	std::cout << delim << std::setfill(' ') << std::endl;
	int whr = ws[0] + ws[1] + ws[2] + delim.length()*2;
	for(unsigned i = 0; i < n; i++) {
		if(i == trunc0) {
			std::cout << std::setfill('.')
				<< delim << std::setw(whr) << ""
				<< delim << std::setfill(' ') << std::endl;
		} else if(i < trunc0 || i >= trunc1) {
			for(unsigned j = 0; j < 3; j++)
				std::cout << delim << std::setw(ws[j])
					<< xss[i][j];
			std::cout << delim << std::endl;
		}
	}

}
