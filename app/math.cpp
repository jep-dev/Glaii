
#include "geometry.hpp"
#include "quaternion.hpp"
#include "dual_quaternion.hpp"
#include "matrix.hpp"
#include "streams.hpp"

///@cond
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>
///@endcond

using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::setprecision;
using std::ostream;
using std::ostringstream;

using std::is_same;

using namespace Geometry;
using Streams::Paster;
using Streams::column;
using Streams::repeat;

ostream& matrices(ostream& dest) {
	Matrix_t<float> mat1 = {
		1, 2, 5, 7,
		0, 1, 3, 6,
		0, 0, 1, 4,
		0, 0, 0, 1
	}, mat2 = {
		 0, 0, 0, 1,
		 0, 0, 1, 0,
		 0, 1, 0, 0,
		-1, 0, 0, 0
	};

	dest << "Matrix products:\n";
	{
		Paster p, pt;
		ostringstream os1, os2, os12, os12t;
		os1 << mat1;
		os2 << mat2;
		os12 << (mat1 * mat2);
		os12t << (mat1 * mat2.transpose());
		//dest << __LINE__ << '\n';
		border(dest, p << os1 << "\n * " << os2
			<< "\n = " << os12);
		//dest << __LINE__ << '\n';
		border(dest, pt << os1 << "\n * " << os2
			<< " T" << "\n = " << os12t);
		//dest << __LINE__ << '\n';
	}
	return dest;
}

ostream& quaternions(ostream& dest) {
	std::vector<Quatf> basis = {1._r, 1._i, 1._j, 1._k};
	{
		Paster paster;
		for(auto const& y : basis) {
			ostringstream col;
			for(auto const& x : basis)
				col << x*y << '\n';
			paster << ' ' << col << ' ';
		}
		dest << "Quaternion product...\n";
		border(dest, paster);
	} {
		Vec_t<float> x = {1,0,0}, y = {0,1,0}, z = {0,0,1};

		static_assert(is_same<decltype(rotation(M_PI, x)),
			Quat_t<double>>::value, "The quaternion should contain "
			"copies of the (deduced) field type.");
		static_assert(is_same<decltype(rotation(float(M_PI), x)),
			decltype(rotation<float>(M_PI, x))>::value,
			"Type conversion should be distributive.");
		// When/how conversion is distributed in the latter case is not
		// specified! Convention, if not control, is due.

		Paster paster;
		float deg90 = M_PI/2;
		auto x90 = rotation(deg90, x),
			 y90 = rotation(deg90, y),
			 z90 = rotation(deg90, z);
		auto header = column(ostringstream(), "", x90, y90, z90),
			 bar = repeat(ostringstream(), "", 1, " | ", 3);
		paster << header;
		for(auto const& point : basis) {
			ostringstream col;
			col << '^' << point << '\n';
			for(auto rot : {x90, y90, z90})
				col << (rot ^ point) << "\n";
			paster << bar << col;
		}
		dest << "... and conjugacy:\n";
		border(dest, paster);
	}
	return dest;
}

ostream& dual_quaternions(ostream& dest) {
	{
		auto z = 0._r, r = 1._r,
			 i = 1._i, j = 1._j, k = 1._k;
		std::vector<DualQuat_t<float>> basis =
			{{r, z}, {i, z}, {j, z}, {k, z},
			 {z, r}, {z, i}, {z, j}, {z, k}};
		{
			Paster paster;
			for(auto j = 0; j < 8; j++) {
				ostringstream col;
				for(auto i = 0; i < 8; i++) {
					col << " " << basis[i]*basis[j] << " \n";
				}
				paster << col;
			}

			dest << "Dual quaternion product...\n";
			border(dest, paster);
		}
		{
			Paster paster;
			DualQuat_t<float> rot = {rotation<float>(M_PI,
				Vec_t<float>{0,0,1}), 0._r}, trans = {1._r, 1._k},
				transrot = trans*rot, rottrans = rot*trans,
				screw = rot+trans;
			std::vector<DualQuat_t<float>>
				xforms = {rot, trans, rottrans, transrot, screw},
				xformed = { {r,i}, {r,j}, {r,k} };
			{
				ostringstream col;
				col << '\n';
				for(auto const& xf : xforms)
					col << xf << '\n';
				paster << col;
			}
			auto bar = repeat(ostringstream(), "", 1, " | ", xforms.size());
			for(auto const& xfd : xformed) {
				ostringstream col;
				col << "^(" << xfd << ")\n";
				for(auto const& xf : xforms) {
					col << (xf ^ xfd) << "\n";
				}
				paster << bar << col << " ";
			}
			dest << "...and conjugacy:\n";
			border(dest, paster);
		}
	}
	return dest;
}

int main(int argc, const char *argv[]) {
	matrices(cout) << endl;
	quaternions(cout) << endl;
	dual_quaternions(cout) << endl;
}
