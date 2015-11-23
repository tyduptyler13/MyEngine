#include "Ray.hpp"
#include "Math.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

template <typename T>
T Ray<T>::distanceSqToSegment(const Vector3<T>& v0, const Vector3<T>& v1, Vector3<T>* optionalPointOnRay, Vector3<T>* optionalPointOnSegment) const {

	Vector3<T> segCenter = (v0 + v1) * 0.5;
	Vector3<T> segDir = (v1 - v0).normalize();
	Vector3<T> diff = origin - segCenter;

	T segExtent = v0.distanceTo(v1) * 0.5;
	T a01 = - direction.dot(segDir);
	T b0 = diff.dot(direction);
	T b1 = - diff.dot(segDir);
	T c = diff.lenghtSq();
	T det = std::abs(1 - a01 * a01);
	T s0, s1, sqrDist, extDet;

	if ( det > 0 ) {

		// The ray and segment are not parallel.

		s0 = a01 * b1 - b0;
		s1 = a01 * b0 - b1;
		extDet = segExtent * det;

		if ( s0 >= 0 ) {

			if ( s1 >= - extDet ) {

				if ( s1 <= extDet ) {

					// region 0
					// Minimum at interior points of ray and segment.

					T invDet = 1 / det;
					s0 *= invDet;
					s1 *= invDet;
					sqrDist = s0 * ( s0 + a01 * s1 + 2 * b0 ) + s1 * ( a01 * s0 + s1 + 2 * b1 ) + c;

				} else {

					// region 1

					s1 = segExtent;
					s0 = Math::max<T>( 0, - ( a01 * s1 + b0 ) );
					sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

				}

			} else {

				// region 5

				s1 = - segExtent;
				s0 = Math::max<T>( 0, - ( a01 * s1 + b0 ) );
				sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

			}

		} else {

			if ( s1 <= - extDet ) {

				// region 4

				s0 = Math::max<T>( 0, - ( - a01 * segExtent + b0 ) );
				s1 = ( s0 > 0 ) ? - segExtent : Math::min<T>(Math::max<T>( - segExtent, - b1 ), segExtent );
				sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

			} else if ( s1 <= extDet ) {

				// region 3

				s0 = 0;
				s1 = Math::min<T>(Math::max<T>( - segExtent, - b1 ), segExtent );
				sqrDist = s1 * ( s1 + 2 * b1 ) + c;

			} else {

				// region 2

				s0 = Math::max<T>( 0, - ( a01 * segExtent + b0 ) );
				s1 = ( s0 > 0 ) ? segExtent : Math::min<T>( Math::max<T>( - segExtent, - b1 ), segExtent );
				sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

			}

		}

	} else {

		// Ray and segment are parallel.

		s1 = ( a01 > 0 ) ? - segExtent : segExtent;
		s0 = Math::max<T>( 0, - ( a01 * s1 + b0 ) );
		sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

	}

	if (optionalPointOnRay) {
		optionalPointOnRay->copy(direction).multiply(s0).add(origin);
	}

	if (optionalPointOnSegment) {
		optionalPointOnSegment->copy(segDir).multiply(s1).add(segCenter);
	}

	return sqrDist;	

}

template <typename T>
std::unique_ptr<Vector3<T>> Ray<T>::intersectTriangle(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, bool backfaceCulling) const {

	Vector3<T> edge1 = b - a;
	Vector3<T> edge2 = c - a;

	Vector3<T> normal = edge1.cross(edge2);

	T ddn = direction.dot(normal);

	int sign;

	if (ddn > 0) {
		if (backfaceCulling) return NULL;
		sign = 1;
	} else if (ddn < 0) {
		sign = -1;
		ddn = -ddn;
	} else {
		return NULL;
	}

	Vector3<T> diff = origin - a;
	T ddqxe2 = sign * direction.dot(diff.cross(edge2));

	if (ddqxe2 < 0) {
		return NULL;
	}

	T dde1xq = sign * direction.dot(edge1.cross(diff));

	if (dde1xq < 0){
		return NULL;
	}

	if (ddqxe2 + dde1xq > ddn) {
		return NULL;
	}

	T qdn = - sign * diff.dot(normal);

	if (qdn < 0){
		return NULL;
	}

	return new Vector3<T>(at(qdn/ddn));

}

