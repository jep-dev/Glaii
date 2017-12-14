#ifndef QUATERNION_TPP
#define QUATERNION_TPP

namespace Geometry {
	template<typename X>
	X Quat_t<X>::operator[](unsigned i) const {
		return (i==0) ? w : (i==1) ? x : (i==2) ? y : (i==3) ? z : X(0);
	}
	template<typename X>
	X Quat_t<X>::operator[](char c) const {
		return c == '1' ? w : c == 'i' ? x : c == 'j'
			? y : c == 'k' ? z : 0;
	}
	template<typename X>
	Quat_t<X> Quat_t<X>::operator*(void) const {
		return {w, -x, -y, -z};
	}
	template<typename X>
	Quat_t<X> Quat_t<X>::operator-(void) const {
		return {-w, -x, -y, -z};
	}
	template<typename X, typename Y = X, typename XY = COMBINE(X,*,Y)>
	XY dot(Quat_t<X> const& l, Quat_t<Y> const& r) {
		return l.w*r.w + l.x*r.x + l.y*r.y + l.z*r.z;
	}
	/*template<typename X>
	X Quat_t<X>::magnitude2(void) const {
		return w*w + x*x + y*y + z*z;
	}
	template<typename X>
	X Quat_t<X>::magnitude(void) const {
		return X(sqrt(magnitude2()));
	}
	template<typename X>
	Quat_t<X> Quat_t<X>::normalize(void) const {
		return *this / magnitude();
	}*/
	template<typename X> template<typename R>
	Quat_t<X> Quat_t<X>::operator=(Quat_t<R> const& r) {
		w = X(r.w); x = X(r.x);
		y = X(r.y); z = X(r.z);
		return *this;
	}
	template<typename X> template<typename R, typename XR>
	Quat_t<XR> Quat_t<X>::operator*(Quat_t<R> const& r) const {
		return {
			XR(w * r.w - x * r.x - y * r.y - z * r.z),
			XR(w * r.x + x * r.w + y * r.z - z * r.y),
			XR(w * r.y - x * r.z + y * r.w + z * r.x),
			XR(w * r.z + x * r.y - y * r.x + z * r.w)
		};
	}
	template<typename X> template<typename R, typename XR>
	Quat_t<XR> Quat_t<X>::operator^(Vec_t<R> const& r) const {
		return Quat_t<XR> {
			XR(          - x * r.x - y * r.y - z * r.z),
			XR(w * r.x             + y * r.z - z * r.y),
			XR(w * r.y - x * r.z             + z * r.x),
			XR(w * r.z + x * r.y - y * r.x            )
		}***this;
	}
	template<typename X> template<typename R, typename XR>
	Quat_t<XR> Quat_t<X>::operator^(Quat_t<R> const& r) const {
		return *this * r * * *this;
	}
	template<typename X> template<typename R, typename XR>
	Quat_t<XR> Quat_t<X>::operator+(Quat_t<R> const& r) const {
		return {XR(w+r.w), XR(x+r.x), XR(y+r.y), XR(z+r.z)};
	}
	template<typename X> template<typename R, typename XR>
	Quat_t<XR> Quat_t<X>::operator-(Quat_t<R> const& r) const {
		return {XR(w-r.w), XR(x-r.x), XR(y-r.y), XR(z-r.z)};
	}
	template<typename X> template<typename R>
	bool Quat_t<X>::operator==(Quat_t<R> const& r) const {
		return w == r.w && x == r.x
			&& y == r.y && z == r.z;
	}
	template<typename X> template<typename R, typename XR>
	Quat_t<XR> Quat_t<X>::operator/(R const& r) const {
		return {w/r, x/r, y/r, z/r};
	}
}

#endif
