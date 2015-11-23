#ifndef MYUPLAY_MYENGINE_CAMERA
#define MYUPLAY_MYENGINE_CAMERA

#include "Object3D.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef OBJECT3D_DEFINED
		template <typename> class Object3D;
		#endif

		template <typename T>
		class Camera : public Object3D<T> {

		public:

			Matrix4<T> matrixWorldInverse;
			Matrix4<T> projectionMatrix;

			Camera() : Object3D<T>() {}

			Camera(const Camera& camera) : Object3D<T>(camera) {
				matrixWorldInverse = camera.matrixWorldInverse;
				projectionMatrix = camera.projectionMatrix;
			}

			Vector3<T> getWorldDirection() const {

				Quaternion<T> quaternion;

				getWorldQuaternion(quaternion);

				return Vector3<T>(0,0,-1).applyQuaternion(quaternion);

			}

			void lookAt(const Vector3<T>& point) {
				Matrix4<T> m1;
				m1.lookAt(this->position, point, this->up);
				this->quaternion.setFromRotationMatrix(m1);
			}

			Camera& copy(const Camera& camera) {

				Object3D<T>::copy(camera);

				matrixWorldInverse = camera.matrixWorldInverse;
				projectionMatrix = camera.projectionMatrix;

				return *this;

			}

			Camera& operator=(const Camera& camera){
				return copy(camera);
			}

		};

		#define CAMERA_DEFINED

	}

}

#endif

