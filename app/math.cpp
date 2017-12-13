
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
using std::ostream;
using std::ostringstream;

using std::is_same;

using namespace Geometry;
using Streams::Paster;
using Streams::column;
using Streams::repeat;

ostream& matrices(ostream& dest) {
	auto theta = M_PI/2;
	float fcos = float(cos(theta)), fsin = float(sin(theta));
	auto mat0 = Matrix_t<decltype(theta)>::identity();
	Matrix_t<float> mat1 = {
		fcos,-fsin, 0, 0,
		fsin, fcos, 0, 0,
		   0,    0, 1, 0,
		   0,    0, 0, 1
	}, mat2 = {
		1,    0,     0, 0,
		0, fcos, -fsin, 0,
		0, fsin,  fcos, 0,
		0,    0,     0, 1
	};
	ostringstream oss1;
	oss1 << mat1;
	auto mat01 = mat0 * mat1;
	auto mat12 = mat1 * mat2;

	dest << "Example of matrix product...\n";
	{
		Paster paster;
		ostringstream oss2, oss12;
		oss2 << mat2;
		oss12 << mat12;
		paster << " " << oss1 << "\n * " << oss2
			<< "\n = " << oss12 << " ";
		border(dest,paster) << "\n";
	}
	dest << "... and transpose\n";
	{
		Paster paster;
		ostringstream oss2;
		oss2 << mat1.transpose();
		paster << oss1 << " T\n  = " << oss2;
		border(dest, paster) << "\n\n";
	}
	dest << "\n\n";
	return dest;
}

ostream& quaternions(ostream& dest) {
	std::vector<Quatf> basis = {1._r, 1._i, 1._j, 1._k};
	{
		Paster paster;
		for(auto const& y : basis) {
			ostringstream col;
			for(auto const& x : basis)
				col << ' ' << x*y << " \n";
			paster << col;
		}
		dest << "Quaternion product:\n";
		ostringstream out;
		border(out, paster);
		dest << out.str() << "\n\n";
	} {
		Vec_t<float> x = {1,0,0}, y = {0,1,0}, z = {0,0,1};

		static_assert(is_same<Quat_t<double>,
			decltype(rotation(M_PI, x))>::value,
			"The double-precision field is copied by the quaternion");
		static_assert(is_same<decltype(rotation<float>(M_PI, x)),
			Quat_t<float>>::value, "The quaternion distributes the loss "
			"of precision to its elements");
		static_assert(is_same<decltype(rotation(float(M_PI), x)),
			Quat_t<float>>::value, "Equivalently, the loss of precision "
			"is copied by the quaternion");

		Paster paster;
		float deg90 = M_PI/2;
		auto x90 = rotation(deg90, x),
			 y90 = rotation(deg90, y),
			 z90 = rotation(deg90, z);
		ostringstream header;
		header << "Rotation\n";
		for(auto rot : {x90, y90, z90})
			header << rot << "\n";

		auto bar = repeat(" | ", 4);
		paster << bar << header << bar;
		for(auto const& point : basis) {
			ostringstream col;
			col << "^(" << point << ")\n";
			for(auto rot : {x90, y90, z90})
				col << (rot ^ point) << "\n";
			paster << col << bar;
		}
		dest << paster << "\n\n";
	}
	return dest;
}

ostream& dual_quaternions(ostream& dest) {
	{
		auto z = 0._r, r = 1._r,
			 i = 1._i, j = 1._j, k = 1._k;
		std::vector<DualQuat_t<float>> basis {
			{r, z}, {i, z}, {j, z}, {k, z},
			{z, r}, {z, i}, {z, j}, {z, k}
		};
		{
			Paster paster;
			for(auto j = 0; j < 8; j++) {
				ostringstream col;
				for(auto i = 0; i < 8; i++) {
					col << " " << basis[i]*basis[j] << " \n";
				}
				paster << col;
			}

			dest << "Dual quaternion product:\n";
			border(dest, paster) << "\n\n";
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
				col << "Transform\n";
				for(auto const& xf : xforms)
					col << xf << "\n";
				paster << col;
			}
			auto bar = repeat("", 1, " | ", xforms.size());
			for(auto const& xfd : xformed) {
				ostringstream col;
				col << "^(" << xfd << ")\n";
				for(auto const& xf : xforms) {
					col << (xf ^ xfd) << "\n";
				}
				paster << bar << col << " ";
			}
			border(dest, paster) << "\n\n";
		}

		/*for(auto i = 5; i < 8; i++) {
			v = basis[i];
			u = {rotation(float(M_PI/2), Vec_t<float>{1,0,0}), {0}};
			dest << "u = " << u << "; v = " << v
					<< "; u^v = " << (u^v) << "\n";
		}*/
	}
	return dest;
}

int main(int argc, const char *argv[]) {
	matrices(cout) << endl;
	quaternions(cout) << endl;
	dual_quaternions(cout) << endl;
}
