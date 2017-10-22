#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "abstract.hpp"

#include <math.h>

#ifndef INTOLERANCE
#define INTOLERANCE 512
#endif

namespace Geometry {

	template<typename> struct Quat_t;
	template<typename> struct Vec_t;

	template<typename X>
	struct Quat_t {
		X x, y, z, w;
		template<typename R>
		Quat_t<X> operator=(Quat_t<R> const& r) {
			x = X(r.x); y = X(r.y);
			z = X(r.z); w = X(r.w);
			return *this;
		}
		Quat_t<X> operator*(void) const {
			return {-x, -y, -z, w};
		}
		Quat_t<X> operator-(void) const {
			return {-x, -y, -z, -w};
		}
		X operator[](unsigned char r) const {
			switch(r) {
				case 'i': case 'x': return x;
				case 'j': case 'y': return y;
				case 'k': case 'z': return z;
				case '1': case 'w': return w;
				default: return 0;
			}
		}
		X magnitude2(void) const {
			return x*x + y*y + z*z + w*w;
		}
		X magnitude(void) const {
			return X(sqrt(magnitude2()));
		}
		Quat_t<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,+,R)>
		Quat_t<XR> operator+(Quat_t<R> const& r) const {
			return {XR(x+r.x), XR(y+r.y), XR(z+r.z), XR(w+r.w)};
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		Quat_t<XR> operator-(Quat_t<R> const& r) const {
			return {XR(x-r.x), XR(y-r.y), XR(z-r.z), XR(w-r.w)};
		}
		template<typename R>
		bool operator==(Quat_t<R> const& r) const {
			return x == r.x && y == r.y
				&& z == r.z && w == r.w;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		Quat_t<XR> operator/(R const& r) const {
			return {x/r, y/r, z/r, w/r};
		}
	};

	template<typename X>
	struct Vec_t {
		X x, y, z;
		X operator[](unsigned char r) const {
			switch(r) {
				case 'i': case 'x': return x;
				case 'j': case 'y': return y;
				case 'k': case 'z': return z;
				default: return 0;
			}
		}
		X magnitude2(void) const {
			return x*x + y*y + z*z;
		}
		X magnitude(void) const {
			return X(sqrt(magnitude2()));
		}
		Vec_t<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		Vec_t<XR> operator-(Vec_t<R> const& r) const {
			return {x-r.x, y-r.y, z-r.z};
		}
		template<typename R>
		bool operator==(Vec_t<R> const& r) const {
			return x == r.x && y == r.y && z == r.z;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		Vec_t<XR> operator/(R const& r) const {
			return {x/r, y/r, z/r};
		}
	};

	template<typename R, typename S, typename T,
		typename RS = COMBINE(R,-,S), typename ST = COMBINE(T,-,S),
		typename RST = COMBINE(RS,*,ST)>
	Vec_t<RST> cross(Vec_t<R> const& l, Vec_t<S> const& c, Vec_t<T> const& r) {
		return (l-c)*(r-c);
	}


	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> rotate(L const& s, Vec_t<R> const& r) {
		LR s2 = LR(s)/2, sc = LR(cos(s2)), ss = LR(sin(s2));
		return Quat_t<LR>{ss*r.x, ss*r.y, ss*r.z, sc};
	}

	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(Quat_t<L> const& l, Quat_t<R> const& r) {
		return {
			LR(l.w * r.x + l.x * r.w + l.y * r.z - l.z * r.y),
			LR(l.w * r.y - l.x * r.z + l.y * r.w + l.z * r.x),
			LR(l.w * r.z + l.x * r.y - l.y * r.x + l.z * r.w),
			LR(l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z)
		};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(Quat_t<L> const& l, Vec_t<R> const& r) {
		return Quat_t<LR> {
			LR(l.w * r.x             + l.y * r.z - l.z * r.y),
			LR(l.w * r.y - l.x * r.z             + l.z * r.x),
			LR(l.w * r.z + l.x * r.y - l.y * r.x            ),
			LR(          - l.x * r.x - l.y * r.y - l.z * r.z)
		}**l;
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(Quat_t<R> const& l, R const& r) {
		return {LR(l.x*r), LR(l.y*r), LR(l.z*r), LR(l.w*r)};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(L const& l, Quat_t<R> const& r) {
		return r*l;
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Vec_t<LR> operator*(Vec_t<L> const& l, Vec_t<R> const& r) {
		return {LR(l.y * r.z - l.z * r.y),
			LR(l.z * r.x - l.x * r.z),
			LR(l.x * r.y - l.y * r.x)};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Vec_t<LR> operator*(Vec_t<R> const& l, L const& r) {
		return {l.x*r, l.y*r, l.z*r};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Vec_t<LR> operator*(L const& l, Vec_t<R> const& r) {
		return r * l;
	}

	template<typename L>
	bool near_zero(L const& l, unsigned prox = INTOLERANCE) {
		auto lp = l * prox;
		return lp >= -1 && lp <= 1;
	}
	template<typename L, typename R>
	bool near(L const& l, R const& r, unsigned prox = INTOLERANCE) {
		return near_zero((r-l).magnitude(), prox);
	}
	template<typename S, typename T>
	S& operator<<(S& dest, Quat_t<T> const& src) {
		bool hit = false;
		T x[] = {src.w, src.x, src.y, src.z};
		const char *l[] = {"", "i", "j", "k"};
		for(unsigned i = 0; i < 4; i++) {
			auto ix = x[i];
			auto il = l[i];
			if(near_zero(ix)) continue;
			hit = true;
			auto ax = float(abs(ix));
			if(i && near_zero(ax - 1))
				dest << ((ix < 0) ? "-" : "+") << il;
			else dest << ix << il;
		}
		if(!hit) dest << "0";
		return dest;
	}
	template<typename S, typename T>
	S& operator<<(S& dest, Vec_t<T> const& src) {
		return dest << Quat_t<T>{src.x, src.y, src.z, 0};
	}

	template struct Quat_t<float>;
	template struct Vec_t<float>;
	template struct Quat_t<double>;
	template struct Vec_t<double>;
}

#endif
