#ifndef MYUPLAY_MYENGINE_FRUSTUM
#define MYUPLAY_MYENGINE_FRUSTUM

#include "Plane.hpp"
#include "Matrix4.hpp"
#include "Object3D.hpp"
#include "Geometry.hpp"

namespace MyUPlay {

	namespace MyEngine {

		//float based. No need for doubles, accuracy isn't that important here.
		class Frustum {

		public:

			Planef planes[6];

			Frustum(){

				for (Planef& plane : planes) {
					plane = Planef();
				}

			};

			//Copy constructor
			Frustum(const Frustum& f) {
				for (unsigned i = 0; i < 6; ++i){
					planes[i] = f.planes[i];
				}
			}

			Frustum& operator=(const Frustum& f) {

				for (unsigned i = 0; i < 6; ++i){
					planes[i] = f.planes[i];
				}

			}

			Frustum& setFromMatrix(const Matrix4f& matrix) {

				auto& me =  matrix.elements me;
				float me0 = me[ 0 ], me1 = me[ 1 ], me2 = me[ 2 ], me3 = me[ 3 ];
				float me4 = me[ 4 ], me5 = me[ 5 ], me6 = me[ 6 ], me7 = me[ 7 ];
				float me8 = me[ 8 ], me9 = me[ 9 ], me10 = me[ 10 ], me11 = me[ 11 ];
				float me12 = me[ 12 ], me13 = me[ 13 ], me14 = me[ 14 ], me15 = me[ 15 ];

				planes[ 0 ].setComponents( me3 - me0, me7 - me4, me11 - me8, me15 - me12 ).normalize();
				planes[ 1 ].setComponents( me3 + me0, me7 + me4, me11 + me8, me15 + me12 ).normalize();
				planes[ 2 ].setComponents( me3 + me1, me7 + me5, me11 + me9, me15 + me13 ).normalize();
				planes[ 3 ].setComponents( me3 - me1, me7 - me5, me11 - me9, me15 - me13 ).normalize();
				planes[ 4 ].setComponents( me3 - me2, me7 - me6, me11 - me10, me15 - me14 ).normalize();
				planes[ 5 ].setComponents( me3 + me2, me7 + me6, me11 + me10, me15 + me14 ).normalize();

				return *this;

			}

			bool interesectsObject(const Object3D<float>& object) const {

				Geometry& geometry = object.geometry;

				if (geometry.boundingSphere == NULL) geometry.computeBoundingSphere();
				Sphere<float> sphere = geometry.boundingSphere;
				sphere.applyMatrix4(object.matrixWorld);

				return intersectsSphere(sphere);

			}

			bool intersectsSphere(const Sphere<float>& sphere) const {

				for (unsigned i = 0; i < 6; ++i) {

					float distance = planes[i].distanceToPoint(sphere.center);

					if (distance < - sphere.radius) {
						return false;
					}

				}

				return true;

			}

			bool intersectsBox(const Box3f& box) const {

				Vector3f p1, p2;

				for (const Planef & plane : planes) {

					p1.x = plane.normal.x > 0 ? box.min.x : box.max.x;
					p2.x = plane.normal.x > 0 ? box.max.x : box.min.x;
					p1.y = plane.normal.y > 0 ? box.min.y : box.max.y;
					p2.y = plane.normal.y > 0 ? box.max.y : box.min.y;
					p1.z = plane.normal.z > 0 ? box.min.z : box.max.z;
					p2.z = plane.normal.z > 0 ? box.max.z : box.min.z;

					float d1 = plane.distanceToPoint(p1);
					float d2 = plane.distanceToPoint(p2);

					if (d1 < 0 && d2 < 0) {
						return false;
					}

				}

				return true;

			}

			bool containsPoint(const Vector3f& point) const {

				for (const Planef& plane : planes) {

					if (plane.distanceToPoint(point) < 0){
						return false;
					}

				}

				return true;

			}

		};

	}

}

#endif

