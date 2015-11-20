#ifndef MYUPLAY_MYENGINE_PERSPECTIVECAMERA
#define MYUPLAY_MYENGINE_PERSPECTIVECAMERA

#include "Camera.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class PerspectiveCamera : public Camera {

			float zoom = 1;

			float fov = 50;
			float aspect = 1;
			float near = 0.1;
			float far = 2000;

			float fullWidth = -1;
			float fullHeight = -1;
			float x = -1;
			float y = -1;
			float width = -1;
			float height = -1;

			PerspectiveCamera() : Camera() {
				updateProjectionMatrix();
			}

			PerspectiveCamera(const PerspectiveCamera& c) : Camera(c) {

				zoom = c.zoom;

				fov = c.fov;
				aspect = c.aspect;
				near = c.near;
				far = c.far;

			}

			void setViewOffset (float fullWidth, float fullHeight, float x, float y, float width, float height){
				this->fullWidth = fullWidth;
				this->fullHeight = fullHeight;
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;

				updateProjectionMatrix();

			}

			void setLens(float focalLength, float frameHeight);
			void updateProjectionMatrix();

			PerspectiveCamera& copy(const PerspectiveCamera& c) {

				Camera::copy(c);

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

		}

	}

}

#endif

