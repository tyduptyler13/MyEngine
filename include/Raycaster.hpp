#include "Ray.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "PerspectiveCamera.hpp"
#include "OrthographicCamera.hpp"
#include "Face3.hpp"
#include "Object3D.hpp"

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

			static T descSort(){

			}

		public:


			Ray<T> ray;
			T near, far;

			Raycaster(Vector3<T> origin, Vector3<T> direction, T near, T far)
			: ray(origin, direction), near(near), far(far) {}

			void set(Vector3<T> origin, Vector3<T> direction) {
				ray.set(origin, direction);
			}

			void setFromCamera(Vector2<T> coords, const PerspectiveCamera<T>& camera) {

				ray.origin.setFromMatrixPosition(camera.matrixWorld);
				ray.direction.set(coords.x, coords.y, 0.5).unproject(camera);

			}

			void setFromCamera(Vector2<T> coords, const OrthographicCamera<T>& camera){

				ray.origin.set(coords.x, coords.y, -1).unproject(camera);
				ray.direction.set(0,0,-1).transformDirection(camera.matrixWorld);

			}

		};

	}

}

