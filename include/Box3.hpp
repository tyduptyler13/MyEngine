#ifndef MYUPLAY_MYENGINE_BOX3
#define MYUPLAY_MYENGINE_BOX3

#include "Box.hpp"
#include "Vector3.hpp"
#include "Object3D.hpp"
#include "Sphere.hpp"

namespace MyUPlay {

	namespace MyEngine {
		
		template <typename T>
		class Box3 : public Box<Vector3, T, Box3<T>> {
			
			protected:
			//Shorthand tricks.

			typedef Box<Vector3, T, Box3<T> > B;
			typedef typename B::limit limit;

			public:

			Vector3<T>& min = B::min;
			Vector3<T>& max = B::max;

			Box3() {
				min(limit::infinity(), limit::infinity());
				max(-limit::infinity(), -limit::infinity());
			}

			Box3(Vector3<T> min, Vector3<T> max) : B(min, max) {}
			Box3(const Vector3<T>& min, const Vector3<T>& max) : B(min, max){}
			
			Box3(const Box3& b) : B(b) {}

			Box3& makeEmpty() {
				min.set(limit::infinity(), limit::infinity(), limit::infinity());
				max.set(-limit::infinity(), -limit::infinity(), -limit::infinity());
				return *this;
			}

			bool empty() {
				return (max.x < min.x) || (max.y < min.y) || (max.z < min.z);
			}

			Box3& setFromObject(const Object3D& object);

			bool containsPoint(const Vector3<T>& point) {

				if (point.x < min.x || point.x > max.x ||
				    point.y < min.y || point.y > max.y ||
				    point.z < min.z || point.z > max.z) {
				
					return false;
				
				}

				return true;

			}
			
			bool containsBox(const Box3& box) {

				if (( min.x <= box.min.x ) && ( box.max.x <= max.x ) &&
				    ( min.y <= box.min.y ) && ( box.max.y <= max.y ) &&
				    ( min.z <= box.min.z ) && ( box.max.z <= max.z )) {

					return true;

				}

				return false;

			}

			Vector3<T>& getParameter(const Vector3<T>& point, Vector3<T>& target) {

				return target.set(
						(point.x - min.x) / (max.x - min.x),
						(point.y - min.y) / (max.y - min.y),
						(point.z - min.z) / (max.z - min.z));

			}

			bool isIntersectionBox(const Box3& box) {

				if ( box.max.x < min.x || box.min.x > max.x ||
				     box.max.y < min.y || box.min.y > max.y ||
				     box.max.z < min.z || box.min.z > max.z ) {

					return false;

				}

				return true;

			}

			Box3& applyMatrix4(const Matrix4&);

			Sphere getBoundingSphere() {
				Sphere s;
				return getBoundingSphere(s);
			}

			Sphere& getBoundingSphere(Sphere& target) {

				target.center = B::center();
				target.radius = B::size().length() * .5;

				return target;

			}


		};

		typedef Box3<float> Box3f;
		typedef Box3<double> Box3d;

	}

}


#endif

