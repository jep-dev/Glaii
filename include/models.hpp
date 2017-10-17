#ifndef MODELS_HPP
#define MODELS_HPP

#include "abstract.hpp"
#include "runnable.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>

// Deduce type from expression, generally for template arg defaults
// TODO more generalized deductions than just infix operators?
#ifndef COMBINE
#define COMBINE(X,OP,Y) decltype(std::declval<X>() OP std::declval<Y>())
#endif

#ifndef INTOLERANCE
#define INTOLERANCE 256
#endif

namespace Models {

	template<typename>
	struct ModelBase;

	template<typename T>
	struct ModelBase: Abstract::Updatable<T> {
		using Abstract::Updatable<T>::update;
	};

	template<typename> struct xyzw;
	template<typename> struct xyz;

	template<typename X>
	struct xyzw {
		X x, y, z, w;
		template<typename R>
		xyzw<X> operator=(xyzw<R> const& r) {
			x = X(r.x); y = X(r.y);
			z = X(r.z); w = X(r.w);
			return *this;
		}
		xyzw<X> operator*(void) const {
			return {-x, -y, -z, w};
		}
		xyzw<X> operator-(void) const {
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
		xyzw<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,+,R)>
		xyzw<XR> operator+(xyzw<R> const& r) const {
			return {XR(x+r.x), XR(y+r.y), XR(z+r.z), XR(w+r.w)};
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		xyzw<XR> operator-(xyzw<R> const& r) const {
			return {XR(x-r.x), XR(y-r.y), XR(z-r.z), XR(w-r.w)};
		}
		template<typename R>
		bool operator==(xyzw<R> const& r) const {
			return x == r.x && y == r.y
				&& z == r.z && w == r.w;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		xyzw<XR> operator/(R const& r) {
			return {x/r, y/r, z/r, w/r};
		}
	};

	template<typename X>
	struct xyz {
		X x,y,z;
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
		xyzw<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		xyz<XR> operator-(xyz<R> const& r) const {
			return {x-r.x, y-r.y, z-r.z};
		}
		template<typename R>
		bool operator==(xyz<R> const& r) const {
			return x == r.x && y == r.y && z == r.z;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		xyz<XR> operator/(R const& r) {
			return {x/r, y/r, z/r};
		}
	};

	template<typename R, typename S, typename T,
		typename RS = COMBINE(R,-,S), typename ST = COMBINE(T,-,S),
		typename RST = COMBINE(RS,*,ST)>
	xyz<RST> cross(xyz<R> const& l, xyz<S> const& c, xyz<T> const& r) {
		return (l-c)*(r-c);
	}


	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyzw<LR> rotate(L const& s, xyz<R> const& r) {
		LR s2 = LR(s)/2, sc = LR(cos(s2)), ss = LR(sin(s2));
		return xyzw<LR>{ss*r.x, ss*r.y, ss*r.z, sc};
	}

	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyzw<LR> operator*(xyzw<L> const& l, xyzw<R> const& r) {
		return {
			LR(l.w * r.x + l.x * r.w + l.y * r.z - l.z * r.y),
			LR(l.w * r.y - l.x * r.z + l.y * r.w + l.z * r.x),
			LR(l.w * r.z + l.x * r.y - l.y * r.x + l.z * r.w),
			LR(l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z)
		};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyzw<LR> operator*(xyzw<L> const& l, xyz<R> const& r) {
		return xyzw<LR> {
			LR(l.w * r.x             + l.y * r.z - l.z * r.y),
			LR(l.w * r.y - l.x * r.z             + l.z * r.x),
			LR(l.w * r.z + l.x * r.y - l.y * r.x            ),
			LR(          - l.x * r.x - l.y * r.y - l.z * r.z)
		}**l;
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyzw<LR> operator*(xyzw<R> const& l, R const& r) {
		return {LR(l.x*r), LR(l.y*r), LR(l.z*r), LR(l.w*r)};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyzw<LR> operator*(L const& l, xyzw<R> const& r) {
		return r*l;
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyz<LR> operator*(xyz<L> const& l, xyz<R> const& r) {
		return {LR(l.y * r.z - l.z * r.y),
			LR(l.z * r.x - l.x * r.z),
			LR(l.x * r.y - l.y * r.x)};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyz<LR> operator*(xyz<R> const& l, L const& r) {
		return {l.x*r, l.y*r, l.z*r};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	xyz<LR> operator*(L const& l, xyz<R> const& r) {
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
	template<typename T>
	std::ostream& operator<<(std::ostream& dest, xyzw<T> const& src) {
		dest << std::showpos;
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
		if(!hit) dest << '0';
		return dest;
	}
	template<typename T>
	std::ostream& operator<<(std::ostream& dest, xyz<T> const& src) {
		return dest << xyzw<T>{src.x, src.y, src.z, 0};
	}
}

#endif
