#ifndef MYUPLAY_MYENGINE_CAMERA
#define MYUPLAY_MYENGINE_CAMERA

#include "Object3D.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Camera : public Object3D<float> {

		public:

			Matrix4<float> matrixWorldInverse;
			Matrix4<float> projectionMatrix;

			Camera() : Object3D<float>() {}

			Camera(const Camera& camera) : Object3D<float>(camera) {
				matrixWorldInverse = camera.matrixWorldInverse;
				projectionMatrix = camera.projectionMatrix;
			}

			Vector3<float> getWorldDirection() const {

				Quaternion<float> quaternion;

				getWorldQuaternion(quaternion);

				return Vector3<float>(0,0,-1).applyQuaternion(quaternion);

			}

			void lookAt(const Vector3<float>& point) {
				Matrix4<float> m1;
				m1.lookAt(this->position, point, this->up);
				this->quaternion.setFromRotationMatrix(m1);
			}

			Camera& copy(const Camera& camera) {

				Object3D<float>::copy(camera);

				matrixWorldInverse = camera.matrixWorldInverse;
				projectionMatrix = camera.projectionMatrix;

				return *this;

			}

			Camera& operator=(const Camera& camera){
				return copy(camera);
			}

		};

	}

}

#endif

