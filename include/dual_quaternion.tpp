#ifndef DUAL_QUATERNION_TPP
#define DUAL_QUATERNION_TPP

namespace Geometry {
	template<typename X>
	Quat_t<X> DualQuat_t<X>::operator[](unsigned i) const {
		return i ? v : u;
	}
	template<typename X> template<typename R>
	DualQuat_t<X> DualQuat_t<X>::operator=(DualQuat_t<R> const& r) {
		u = r.u; v = r.v;
		return *this;
	}
	template<typename X>
	DualQuat_t<X> DualQuat_t<X>::operator*(void) const {
		return {*u, *v};
	}
	template<typename X>
	DualQuat_t<X> DualQuat_t<X>::operator-(void) const {
		return {-u, -v};
	}
	template<typename X> template<typename R, typename XR>
	DualQuat_t<XR> DualQuat_t<X>::operator+(DualQuat_t<R> const& r) const {
		return {u + r.u, v + r.v};
	}
	template<typename X> template<typename R, typename XR>
	DualQuat_t<XR> DualQuat_t<X>::operator-(DualQuat_t<R> const& r) const {
		return {u - r.u, v - r.v};
	}
	template<typename X> template<typename R>
	bool DualQuat_t<X>::operator==(DualQuat_t<R> const& r) const {
		return u == r.u && v == r.v;
	}
	template<typename X> template<typename R, typename XR>
	DualQuat_t<XR> DualQuat_t<X>::operator/(R const& r) const {
		return {u/r, v/r};
	}
	template<typename L, typename R, typename LR>
	LR dot(DualQuat_t<L> const& l, DualQuat_t<R> const& r) {
		return dot(l.u, r.u) + dot(l.v, r.v);
	}
	template<typename L, typename R, typename LR>
	DualQuat_t<LR> operator*(DualQuat_t<L> const& l, DualQuat_t<R> const& r) {
		return {l.u*r.u, l.u*r.v + l.v*r.u};
	}
	template<typename L, typename R, typename LR>
	DualQuat_t<LR> operator*(DualQuat_t<L> const& l, Quat_t<R> const& r) {
		return {l.u*r, l.v*r};
	}
	template<typename L, typename R, typename LR>
	DualQuat_t<LR> operator*(L const& l, DualQuat_t<R> const& r) {
		return {l*r.u, l*r.v};
	}
	template<typename L, typename R, typename LR>
	DualQuat_t<LR> operator^(DualQuat_t<L> const& l, DualQuat_t<R> const& r) {
		return DualQuat_t<LR>{l.u*r.u, l.u*r.v + l.v*r.u} * *l;
	}
	template<typename L, typename R, typename LR>
	DualQuat_t<LR> operator^(DualQuat_t<L> const& l, Quat_t<R> const& r) {
		return DualQuat_t<LR>{l.u*r, l.v*r} * *l;
	}
}

#endif
