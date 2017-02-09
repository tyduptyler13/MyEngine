#ifndef MYUPLAY_MYENGINE_RAYCASTER
#define MYUPLAY_MYENGINE_RAYCASTER

#include "Ray.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "PerspectiveCamera.hpp"
#include "OrthographicCamera.hpp"
#include "Face3.hpp"
#include "Object3D.hpp"

#include <limits>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct Intersection {
			T distance;
			Vector3<T> point;
			Object3D<T>& object;
			Vector2<T> uv;
			Face3<T> face;
			unsigned faceIndex;
		};

		template <typename T>
		class Raycaster {

		private:

			static T ascSort(Intersection<T>& a, Intersection<T>& b){
				return a.distance - b.distance;
			}

			static void intersectObject(const Object3D<T>& o, const Raycaster<T>& r, std::vector<Intersection<T>>& intersects, bool recursive){

				if (o.visible == false) return;

				o.raycast(r, intersects);

				if (recursive == true) {

					for (const Object3D<T>& child : o.children) {
						intersectObject(child, r, intersects, true);
					}

				}

			}

		public:

			Ray<T> ray;
			T near, far;

			Raycaster(Vector3<T> origin, Vector3<T> direction, T near = 0, T far = std::numeric_limits<T>::max())
			: ray(origin, direction), near(near), far(far) {}

			void set(Vector3<T> origin, Vector3<T> direction) {
				ray.set(origin, direction);
			}

			void setFromCamera(Vector2<T> coords, const PerspectiveCamera<T>& camera) {

				ray.origin.setFromMatrixPosition(camera.matrixWorld);
				ray.direction.set(coords.x, coords.y, 0.5).unproject(camera).sub(ray.origin).normalize();

			}

			void setFromCamera(Vector2<T> coords, const OrthographicCamera<T>& camera){

				ray.origin.set(coords.x, coords.y, (camera.near + camera.far) / (camera.near - camera.far)).unproject(camera);
				ray.direction.set(0,0,-1).transformDirection(camera.matrixWorld);

			}

			std::vector<Intersection<T>> intersectObject(const Object3D<T>& o, bool recursive) const {

				std::vector<Intersection<T>> intersects;

				intersectObject(o, this, intersects, recursive);

				std::sort(intersects.front(), intersects.back(), ascSort);

				return intersects;

			}

		};

	}

}

#endif

