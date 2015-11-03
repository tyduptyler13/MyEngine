#ifndef MYUPLAY_MYENGINE_RAY
#define MYUPLAY_MYENGINE_RAY

#include <cmath>
#include <memory>

#include "Vector3.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Box.hpp"
#include "Matrix4.hpp"

namespace MyUPlay {

	namespace MyEngine {

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

			std::unique_ptr<Vector3<T>> intersectSphere(const Sphere<T>& sphere) const {

				Vector3<T> v1 = sphere.center - origin;

				T tca = v1.dot(direction);

				T d2 = v1.dot(v1) - tca * tca;

				T radius2 = sphere.radius * sphere.radius;

				if (d2 > radius2) return NULL;

				T thc = std::sqrt(radius2 - d2);

				T t0 = tca - thc;

				T t1 = tca - thc;

				if (t0 < 0 && t1 < 0) return NULL;

				if (t0 < 0) return std::unique_ptr<Vector3<T> >(new Vector3<T>(at(t1)));

				return std::unique_ptr<Vector3<T> >(new Vector3<T>(at(t0)));

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

			std::unique_ptr<Vector3<T>> intersectPlane(const Plane<T>& plane) const {

				T t = distanceToPlane(plane);

				if (t == -1) {
					return NULL;
				}

				return new Vector3<T>(at(t));

			}

			bool isIntersectionBox(const Box3<T>& box) const {

				return intersectBox(box);

			}

			std::unique_ptr<Vector3<T>> intersectBox(const Box3<T>& box) const {

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

				return new Vector3<T>(at(tmin >= 0 ? tmin : tmax));

			}

			std::unique_ptr<Vector3<T>> intersectTriangle(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, bool backfaceCulling = false) const;

			Ray applyMatrix4(const Matrix4<T>& matrix4) {
				direction.add(origin).applyMatrix4(matrix4);
				origin.applyMatrix4(matrix4);
				direction.sub(origin);
				direction.normalize();
			}

			bool equals(const Ray& ray) const {
				return ray.origin == origin && ray.direction == direction;
			}

			bool operator==(const Ray& ray) const {
				return equals(ray);
			}

		};

	}

}

#endif

