#ifndef MYUPLAY_MYENGINE_PLANE
#define MYUPLAY_MYENGINE_PLANE

#include "Vector3.hpp"
#include "Sphere.hpp"
#include "Line.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Plane {

		public:

			Vector3f normal;
			float constant;

			Plane() : normal(1,0,0), constant(0) {}
			Plane(const Vector3f& normal, float constant = 0) : normal(normal), constant(constant) {}
			Plane(const Plane& p){
				normal = p.normal;
				constant = p.constant;
			}

			Plane& set(const Vector3f& normal, float constant){
				this->normal = normal;
				this->constant = constant;

				return *this;
			}

			Plane& setComponents(float x, float y, float z, float w){
				normal.set(x, y, z);
				constant = w;

				return *this;
			}

			Plane& setFromNormalAndCoplanarPoint(const Vector3f& normal, const Vector3f& point) {
				this->normal.copy(normal);
				constant = - point.dot(this->normal);
				return *this;
			}

			Plane& setFromCoplanarPoints(const Vector3f& a, const Vector3f& b, const Vector3f& c){
				Vector3f normal = (c - b).cross(a - b).normalize();
				setFromNormalAndCoplanarPoint(normal, a);
				return *this;
			}

			Plane& copy(const Plane& p){
				normal = p.normal;
				constant = p.constant;
				return *this;
			}

			Plane& normalize() {
				constant *= -1;
				normal.negate();
				return *this;
			}

			float distanceToPoint(const Vector3f& point) const {
				return normal.dot(point) + constant;
			}

			float distanceToSphere(const Sphere<float>& sphere) const {
				return distanceToPoint(sphere.center) - sphere.radius;
			}

			Vector3f& projectPoint(const Vector3f& point, Vector3f& target) const {
				return orthoPoint(point, target).sub(point).negate();
			}

			Vector3f projectPoint(const Vector3f& point) const {
				Vector3f t;
				return projectPoint(point, t);
			}

			Vector3f& orthoPoint(const Vector3f& point, Vector3f& target) const {
				float perpendicularMagnitude = distanceToPoint(point);
				return target.copy(normal) *= perpendicularMagnitude;
			}

			Vector3f orthoPoint(const Vector3f& point) const {
				Vector3f t;
				return orthoPoint(point, t);
			}

			Vector3f isIntersectionLine(const Line3f& line) {

			}

		};

	}

}

#endif

