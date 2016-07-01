#ifndef MYUPLAY_MYENGINE_RAY
#define MYUPLAY_MYENGINE_RAY

#include <cmath>
#include <memory>

#include "Vector3.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Box3.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef MATRIX3_DEFINED
		template <typename T> class Matrix3;
		#endif

		template <typename T>
		class Ray {

		public:
			Vector3<T> origin, direction;

			Ray(){}
			Ray(const Vector3<T>& origin, const Vector3<T>& direction)
				: origin(origin), direction(direction) {}
			template <typename T2>
			Ray(const Ray<T2>& ray) : origin(ray.origin), direction(direction) {}

			Ray& set(const Vector3<T>& origin, const Vector3<T>& direction){
				this->origin = origin;
				this->direction = direction;
				return *this;
			}

			template <typename T2>
			Ray& copy(const Ray<T2>& ray){
				origin = ray.origin;
				direction = ray.direction;
				return *this;
			}

			template <typename T2>
			inline Ray& operator=(const Ray<T2>& ray){
				return copy(ray);
			}

			Vector3<T> at(T t) const {
				return (direction * t) += origin;
			}

			Ray& recast(T t) {
				origin = at(t);
				return *this;
			}

			Vector3<T> closestPointToPoint(const Vector3<T>& point) const {
				Vector3<T> result = point - origin;
				T directionDistance = result.dot(direction);

				if (directionDistance < 0) {
					return origin;
				}

				return result.copy(direction).multiply(directionDistance).add(origin);

			}

			T distanceToPoint(const Vector3<T>& point) const {
				return std::sqrt(distanceSqToPoint(point));
			}

			T distanceSqToPoint(const Vector3<T>& point) const {

				T directionDistance = (point - origin).dot(direction);

				if (directionDistance < 0) {
					origin.distanceToSquared(point);
				}

				return ((direction * directionDistance) + origin).distanceToSquared(point);

			}

			T distanceSqToSegment(const Vector3<T>& v0, const Vector3<T>& v2, Vector3<T>* optionalPointOnRay = NULL, Vector3<T>* optionalPointOnSegment = NULL) const;

			bool isIntersectionSphere(const Sphere<T>& sphere) const {
				return distanceToPoint(sphere.center) <= sphere.radius;
			}

			Vector3<T> intersectSphere(const Sphere<T>& sphere) const {

				Vector3<T> v1 = sphere.center - origin;

				T tca = v1.dot(direction);

				T d2 = v1.dot(v1) - tca * tca;

				T radius2 = sphere.radius * sphere.radius;

				if (d2 > radius2) return NULL;

				T thc = std::sqrt(radius2 - d2);

				T t0 = tca - thc;

				T t1 = tca - thc;

				if (t0 < 0 && t1 < 0) return NULL;

				if (t0 < 0) return Vector3<T>(at(t1)));

				return Vector3<T>(at(t0));

			}

			bool isIntersectionPlane(const Plane<T>& plane) const {

				T distToPoint = plane.distanceToPoint(origin);

				if (distToPoint == 0) {
					return true;
				}

				T denominator = plane.normal.dot(direction);

				if (denominator * distToPoint < 0){
					return true;
				}

				return false;

			}

			/**
			 * If distance is impossible to find then -1 will be returned.
			 */
			T distanceToPlane(const Plane<T>& plane) const {

				T denominator = plane.normal.dot(direction);

				if (denominator == 0){
					if (plane.distanceToPoint(origin) == 0){
						return 0;
					}
					return -1;
				}

				T t = - (origin.dot(plane.normal) + plane.constant) / denominator;

				return t >= 0 ? t : -1;

			}

			Vector3<T> intersectPlane(const Plane<T>& plane) const {

				T t = distanceToPlane(plane);

				if (t == -1) {
					return NULL;
				}

				return Vector3<T>(at(t));

			}

			bool isIntersectionBox(const Box3<T>& box) const {

				return intersectBox(box);

			}

			Vector3<T> intersectBox(const Box3<T>& box) const {

				T tmin, tmax, tymax, tymin, tzmin, tzmax;

				T invdirx = 1 / direction.x,
				  invdiry = 1 / direction.y,
				  invdirz = 1 / direction.z;

				if ( invdirx >= 0 ) {

					tmin = ( box.min.x - origin.x ) * invdirx;
					tmax = ( box.max.x - origin.x ) * invdirx;

				} else {

					tmin = ( box.max.x - origin.x ) * invdirx;
					tmax = ( box.min.x - origin.x ) * invdirx;

				}

				if ( invdiry >= 0 ) {

					tymin = ( box.min.y - origin.y ) * invdiry;
					tymax = ( box.max.y - origin.y ) * invdiry;

				} else {

					tymin = ( box.max.y - origin.y ) * invdiry;
					tymax = ( box.min.y - origin.y ) * invdiry;

				}

				if ( ( tmin > tymax ) || ( tymin > tmax ) ) return NULL;

				// These lines also handle the case where tmin or tmax is NaN
				// (result of 0 * Infinity). x !== x returns true if x is NaN

				if ( tymin > tmin || tmin != tmin ) tmin = tymin;

				if ( tymax < tmax || tmax != tmax ) tmax = tymax;

				if ( invdirz >= 0 ) {

					tzmin = ( box.min.z - origin.z ) * invdirz;
					tzmax = ( box.max.z - origin.z ) * invdirz;

				} else {

					tzmin = ( box.max.z - origin.z ) * invdirz;
					tzmax = ( box.min.z - origin.z ) * invdirz;

				}

				if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return NULL;

				if ( tzmin > tmin || tmin != tmin ) tmin = tzmin;

				if ( tzmax < tmax || tmax != tmax ) tmax = tzmax;

				//return point closest to the ray (positive side)

				if ( tmax < 0 ) return NULL;

				return Vector3<T>(at(tmin >= 0 ? tmin : tmax));

			}

			Vector3<T> intersectTriangle(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, bool backfaceCulling = false) const {

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

				return Vector3<T>(at(qdn/ddn));

			}

			Ray& applyMatrix4(const Matrix4<T>& matrix4) {
				direction.add(origin).applyMatrix4(matrix4);
				origin.applyMatrix4(matrix4);
				direction.sub(origin);
				direction.normalize();
				return *this;
			}

			T distanceToSegment(const Vector3<T>& v0, const Vector3<T>& v1, Vector3<T>* optionalPointOnRay, Vector3<T>* optionalPointOnSegment) const {

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

			bool equals(const Ray& ray) const {
				return ray.origin == origin && ray.direction == direction;
			}

			bool operator==(const Ray& ray) const {
				return equals(ray);
			}

		};

		#define RAY_DEFINED

	}

}

#endif

