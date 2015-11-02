#ifndef MYUPLAY_MYENGINE_RAY
#define MYUPLAY_MYENGINE_RAY

#include <cmath>

#include "Vector3.hpp"
#include "Math.hpp"

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

			T distanceToSegment(const Vector3<T>& v0, const Vector3<T>& v2, Vector3<T>* optionalPointOnRay = NULL, Vector3<T>* optionalPointOnSegment = NULL) const;


		};

	}

}

#endif

