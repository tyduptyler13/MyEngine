#ifndef MYUPLAY_MYENGINE_SPHERE
#define MYUPLAY_MYENGINE_SPHERE

#include <cmath>
#include <vector>

#include "Vector3.hpp"
#include "Box3.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef BOX3_DEFINED
		template <typename T> class Box3;
		#endif
		#ifndef OBJECT3D_DEFINED
		template <typename T> class Object3D;
		#endif

		template <typename T>
		class Sphere {

		public:

			Vector3<T> center;
			T radius = 0;

			Sphere(Vector3<T> center = Vector3<T>(), T radius = 0) : center(center), radius(radius) {}
			Sphere(const Sphere& sphere) : center(sphere.center), radius(sphere.radius) {}

			Sphere& set(const Vector3<T>& center, T radius) {
				this->center.copy(center);
				this->radius = radius;

				return *this;
			}

			Sphere& setFromPoints(const std::vector<const Vector3<T>>& points, const Vector3<T>& center) {

				T maxRadiusSq = 0;

				for (const Vector3<T>& point : points) {
					maxRadiusSq = Math::max<T>(maxRadiusSq, center.distanceToSquared(point));
				}

				radius = std::sqrt(maxRadiusSq);

				return *this;

			}

			Sphere& setFromPoints(std::vector<Vector3<T>> points) {

				Vector3<T> center;

				Box3<T>().setFromPoints(points, center);

				return setFromPoints(points, center);


			}

			Sphere& copy(const Sphere& sphere) {
				center = sphere.center;
				radius = sphere.radius;

				return *this;
			}

			Sphere& operator=(const Sphere& sphere) {
				return copy(sphere);
			}

			bool empty() const {
				return radius <= 0;
			}

			bool containsPoint(const Vector3<T>& point) const {

				return point.distanceToSquared(center) <= radius * radius;

			}

			T distanceToPoint(const Vector3<T>& point) const {

				return point.distanceTo(center) - radius;

			}

			bool intersectsSphere(const Sphere& sphere) const {

				T radiusSum = radius + sphere.radius;

				return sphere.center.distanceToSquared(center) <= radiusSum * radiusSum;

			}

			Vector3<T> clampPoint(const Vector3<T>& point) const {

				T deltaLengthSq = center.distanceToSquared(point);

				Vector3<T> result(point);

				if (deltaLengthSq > ( radius * radius )) {

					result.sub(center).normalize();
					result.multiply(radius).add(center);

				}

				return result;

			}

			Box3<T> getBoundingBox() const {

				Box3<T> box(center, radius);

				box.expandByScalar(radius);

				return box;

			}

			Sphere& applyMatrix4(const Matrix4<T>& matrix) {

				center.applyMatrix4(matrix);
				radius = radius * matrix.getMaxScaleOnAxis();

				return *this;

			}

			Sphere& translate(const Vector3<T>& offset) {

				center.add(offset);

				return *this;

			}

			bool equals(const Sphere& sphere) const {

				return center == sphere.center && radius == sphere.radius;

			}

			bool operator==(const Sphere& sphere) const {
				return equals(sphere);
			}

		};

		typedef Sphere<float> Spheref;
		typedef Sphere<double> Sphered;

		#define SPHERE_DEFINED

	}

}

#endif

