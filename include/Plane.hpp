#ifndef MYUPLAY_MYENGINE_PLANE
#define MYUPLAY_MYENGINE_PLANE

#include <stdexcept>

#include "Vector3.hpp"
#include "Sphere.hpp"
#include "Line3.hpp"
#include "Matrix4.hpp"
#include "Matrix3.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T = float>
		class Plane {

		public:

			Vector3<T> normal;
			T constant;

			Plane() : normal(1,0,0), constant(0) {}
			Plane(const Vector3<T>& normal, T constant = 0) : normal(normal), constant(constant) {}
			Plane(const Plane& p){
				normal = p.normal;
				constant = p.constant;
			}

			Plane& set(const Vector3<T>& normal, T constant){
				this->normal = normal;
				this->constant = constant;

				return *this;
			}

			Plane& setComponents(T x, T y, T z, T w){
				normal.set(x, y, z);
				constant = w;

				return *this;
			}

			Plane& setFromNormalAndCoplanarPoint(const Vector3<T>& normal, const Vector3<T>& point) {
				this->normal.copy(normal);
				constant = - point.dot(this->normal);
				return *this;
			}

			Plane& setFromCoplanarPoints(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c){
				Vector3<T> normal = (c - b).cross(a - b).normalize();
				setFromNormalAndCoplanarPoint(normal, a);
				return *this;
			}

			Plane& copy(const Plane& p){
				normal = p.normal;
				constant = p.constant;
				return *this;
			}

			Plane& normalize() {
				T inverseNormalLength = 1.0 / normal.length();
				normal *= inverseNormalLength;
				constant *= inverseNormalLength;
				return *this;
			}

			T distanceToPoint(const Vector3<T>& point) const {
				return normal.dot(point) + constant;
			}

			T distanceToSphere(const Sphere<T>& sphere) const {
				return distanceToPoint(sphere.center) - sphere.radius;
			}

			Vector3<T>& projectPoint(const Vector3<T>& point, Vector3<T>& target) const {
				return orthoPoint(point, target).sub(point).negate();
			}

			Vector3<T> projectPoint(const Vector3<T>& point) const {
				Vector3<T> t;
				return projectPoint(point, t);
			}

			Vector3<T>& orthoPoint(const Vector3<T>& point, Vector3<T>& target) const {
				T perpendicularMagnitude = distanceToPoint(point);
				return target.copy(normal) *= perpendicularMagnitude;
			}

			Vector3<T> orthoPoint(const Vector3<T>& point) const {
				Vector3<T> t;
				return orthoPoint(point, t);
			}

			bool isIntersectionLine(const Line3<T>& line) const {
				T startSign = distanceToPoint(line.start);
				T endSign = distanceToPoint(line.end);
				return (startSign < 0 && endSign > 0) || (endSign < 0 && startSign > 0);
			}

			Vector3<T>& intersectLine(const Line3<T>& line, Vector3<T>& target) const {
				Vector3<T> direction = line.delta();
				T denominator = normal.dot(direction);

				if (denominator == 0){
					if (distanceToPoint(line.start) == 0) {
						return target.copy(line.start);
					}

					throw std::logic_error("Parallel intersection line. No solution.");
				}

				T t = - (line.start.dot(normal) + constant) / denominator;

				if (t < 0 || t > 1) {
					throw std::logic_error("Line does not intersect the plane.");
				}

				return target.copy(direction).multiply(t).add(line.start);

			}

			Vector3<T> intersectLine(const Line3<T>& line) const {
				Vector3<T> v;
				return intersectLine(line, v);
			}

			inline Vector3<T> coplanarPoint() const {
				return Vector3<T>(normal).multiply(-constant);
			}

			Plane& applyMatrix4(const Matrix4<T>& matrix, const Matrix3<T>& normalMatrix) {

				Vector3<T> newNormal = Vector3<T>(normal).applyMatrix3(normalMatrix);

				Vector3<T> newCopanarPoint = coplanarPoint();

				newCopanarPoint.applyMatrix4(matrix);

				setFromNormalAndCoplanarPoint(newNormal, newCopanarPoint);

				return *this;

			}

			Plane& applyMatrix4(const Matrix4<T>& matrix) {

				Matrix3<T> m1;

				return applyMatrix4(matrix, m1.getNormalMatrix(matrix));

			}

			Plane& translate(const Vector3<T>& offset) {

				constant = constant - offset.dot(normal);

				return *this;

			}

			bool equals(const Plane& plane) const {
				return normal == plane.normal && constant == plane.constant;
			}

			inline bool operator=(const Plane& plane) const {
				return equals(plane);
			}

		};

		typedef Plane<float> Planef;
		typedef Plane<double> Planed;

	}

}

#endif

