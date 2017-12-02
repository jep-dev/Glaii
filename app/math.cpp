#include "geometry.hpp"
#include "quaternion.hpp"
#include "dual_quaternion.hpp"
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
	using std::is_same;
	using Streams::Paster;
	using Streams::column;
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
		   0, 1,    0, 0,
		fsin, 0, fcos, 0,
		   0, 0,    0, 1
	};
	auto mat01 = mat0 * mat1;
	auto mat12 = mat1 * mat2;

	static_assert(is_same<decltype(mat01), decltype(mat0)>::value,
		"LCM(double, float) should be double");

	OSS oss1;
	oss1 << mat1;
	{
		Paster paster;
		OSS oss2, oss12;
		oss2 << mat2;
		oss12 << mat12;
		paster << oss1 << "\n * " << oss2 << "\n = " << oss12;
		dest << paster;
	}
	endl(dest);
	{
		Paster paster;
		OSS oss2;
		oss2 << mat1.transpose();
		paster << oss1 << " T\n  = " << oss2;
		dest << paster;
	}
	endl(dest);
	return dest;
}

OS& quaternions(OS& dest) {
	using Streams::Paster;
	using Streams::column;
	using std::is_same;
	{
		Quat_t<float> x[] = {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
		};

		Paster paster;
		OSS col;
		paster.repeat("", 1, "| ", 4)
			<< column("", x[0], x[1], x[2], x[3]);
		paster.repeat("", 1, " | ", 4);
		for(auto i = 0; i < 4; i++)
			paster << (i ? " " : "")
				<< column(x[i], x[0]*x[i],
					x[1]*x[i], x[2]*x[i], x[3]*x[i]);
		dest << paster;
	}

	{
		Paster paster;
		Vec_t<float> x = {1,0,0}, y = {0,1,0}, z = {0,0,1};
		Quat_t<float> ident = {0,0,0,1};
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

		float deg90 = M_PI/2;
		const char *szAxes[] = {"e_i", "e_j", "e_k"},
			*szRots[] = {"* e_i, ", "* e_j, ", "* e_k"};
		Quat_t<float> rotAxes[] =
			{ rotate(deg90, x), rotate(deg90, y), rotate(deg90, z) };
		paster.column("Axis", szAxes[0], szAxes[1], szAxes[2])
			.repeat("", 1, " -> ", 3)
			.column("1/4 turn", rotAxes[0], rotAxes[1], rotAxes[2])
			.repeat(" ", 4)
			.column(szRots[0], rotAxes[0]*x, rotAxes[1]*x, rotAxes[2]*x)
			.repeat(" ", 4)
			.column(szRots[1], rotAxes[0]*y, rotAxes[1]*y, rotAxes[2]*y)
			.repeat(" ", 4)
			.column(szRots[2], rotAxes[0]*z, rotAxes[1]*z, rotAxes[2]*z);
		dest << paster;
	}
	return dest;
}

int main(int argc, const char *argv[]) {
	quaternions(cout) << endl;
	matrices(cout) << endl;
}
