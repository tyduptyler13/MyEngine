#ifndef MYUPLAY_MYENGINE_ORTHOGRAPHICCAMERA
#define MYUPLAY_MYENGINE_ORTHOGRAPHICCAMERA

#include "Camera.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef CAMERA_DEFINED
		template <typename T> class Camera;
		#endif

		template <typename T>
		class OrthographicCamera : public Camera<T> {

		public:

			T left, right, top, bottom, near, far;

			T zoom = 1;

			OrthographicCamera(T left, T right, T top, T bottom, T near, T far)
				: Camera<T>(), left(left), right(right), top(top), bottom(bottom), near(near), far(far){
				updateProjectionMatrix();
			}

			template <typename T2>
			OrthographicCamera(const OrthographicCamera<T2>& camera)
				: Camera<T>(camera), left(camera.left), right(camera.right), top(camera.top), bottom(camera.bottom), near(camera.near), far(camera.far) {
				zoom = camera.zoom;
			}

			void updateProjectionMatrix(){

				T dx = (right - left) / (2 * zoom),
				  dy = (top - bottom) / (2 * zoom),
				  cx = (right + left) / 2,
				  cy = (top + bottom) / 2;

				this->projectionMatrix.makeOrthographic(cx - dx, cx + dx, cy + dy, cy - dy, near, far);

			}

			template <typename T2>
			OrthographicCamera& copy(const OrthographicCamera<T2>& camera) {

				Camera<T>::copy(camera);

				left = camera.left;
				right = camera.right;
				top = camera.top;
				bottom = camera.bottom;
				near = camera.near;
				far = camera.far;

				zoom = camera.zoom;

				return *this;

			}

		};

	}

}

#endif

