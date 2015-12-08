#ifndef MYUPLAY_MYENGINE_PERSPECTIVECAMERA
#define MYUPLAY_MYENGINE_PERSPECTIVECAMERA

#include "Camera.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef CAMERA_DEFINED
		template <typename T> class Camera;
		#endif

		template <typename T>
		class PerspectiveCamera : public Camera<T> {

			T zoom = 1;

			T fov = 50;
			T aspect = 1;
			T near = 0.1;
			T far = 2000;

			T fullWidth = -1;
			T fullHeight = -1;
			T x = -1;
			T y = -1;
			T width = -1;
			T height = -1;

			PerspectiveCamera() : Camera<T>() {
				updateProjectionMatrix();
			}

			PerspectiveCamera(const PerspectiveCamera& c) : Camera<T>(c) {

				zoom = c.zoom;

				fov = c.fov;
				aspect = c.aspect;
				near = c.near;
				far = c.far;

			}

			void setViewOffset (T fullWidth, T fullHeight, T x, T y, T width, T height){
				this->fullWidth = fullWidth;
				this->fullHeight = fullHeight;
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;

				updateProjectionMatrix();

			}

			void setLens(T focalLength, T frameHeight);
			void updateProjectionMatrix();

			PerspectiveCamera& copy(const PerspectiveCamera& c) {

				Camera<T>::copy(c);

				fov = c.fov;
				aspect = c.fov;
				near = c.near;
				far = c.far;

				zoom = c.zoom;

				return *this;

			}

			inline PerspectiveCamera& operator=(const PerspectiveCamera& c){
				copy(c);
			}

		};

		#define PERSPECTIVECAMERA_DEFINED

	}

}

#endif

