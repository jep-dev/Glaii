#include "geometry.hpp"
#include "matrix.hpp"
#include "streams.hpp"

#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::setprecision;
using OS = std::ostream;
using OSS = std::ostringstream;

using namespace Geometry;

OS& matrices(OS& dest) {
	using Streams::paste;
	using Streams::border;
	using Streams::pasteBorder;
	auto theta = M_PI/2;
	float fcos = float(cos(theta)), fsin = float(sin(theta));
	auto mat0 = Matrix_t<decltype(theta)>::identity();
	Matrix_t<float> mat1 = {
		fcos,-fsin, 0, 0,
		fsin, fcos, 0, 0,
		   0,    0, 1, 0,
		   0,    0, 0, 1
	}, mat2 = {
		fcos, 0,-fsin, 0,
		0,    1,    0, 0,
		fsin, 0, fcos, 0,
		0,    0,    0, 1
	};
	auto mat01 = mat0 * mat1;
	auto mat12 = mat1 * mat2;

	static_assert(std::is_same<
			decltype(mat01), decltype(mat0)
		>::value, "LCM(double, float) should be double");

	auto wMat = 11, wBord = 3;
	{
		auto accum = 0;
		OSS lhs, rhs, res,
			bordLhs, bordRhs, bordRes, out01, out12;
		lhs << mat1;
		rhs << mat2;
		res << mat12;
		border(bordLhs, lhs, wMat, 2, 2);
		border(bordRhs, rhs, wMat, 0, 2);
		border(bordRes, res, wMat, 2, 2);
		paste(out01, bordLhs, wMat+4, bordRhs, wMat+2, 0);
		paste(out12, out01.str(), wMat*2+6, "\n\n = ", 3, 0);
		paste(dest, out12, wMat*2+9, bordRes, wMat+4, 0);
	}


	{
		auto accum = 0;
		OSS lhs, res, bordLhs, bordRes, out2, out3;
		lhs << mat12;
		res << mat12.transpose();
		border(bordLhs, lhs, wMat, 2, 2);
		border(bordRes, res, wMat, 2, 2);
		paste(out2, bordRes.str(), accum += wMat+4, "T\n\n = ", 3, 0);
		paste(dest, out2, accum+3, bordLhs, wMat+4, 0);
	}

	return dest;
}

OS& table(OS& dest) {
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
			Quat_t<double>, decltype(rotate(M_PI, x))
		>::value, "LCM(float, double) should be double");
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
	matrices(cout);
}
