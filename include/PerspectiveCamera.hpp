#ifndef MYUPLAY_MYENGINE_PERSPECTIVECAMERA
#define MYUPLAY_MYENGINE_PERSPECTIVECAMERA

#include <cmath>

namespace MyUPlay {
	namespace MyEngine {
		template <typename T> struct PerspectiveCamera;
	}
}

#include "Math.hpp"
#include "Camera.hpp"

template <typename T>
struct MyUPlay::MyEngine::PerspectiveCamera : public Camera<T> {

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

	PerspectiveCamera(T fov = 50, T aspect = 1, T near = 0.1, T far = 2000) : Camera<T>(),
			fov(fov), aspect(aspect), near(near), far(far) {
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

	void setLens(T focalLength, T frameHeight){

		fov = 2 * Math::radToDeg<T>(std::atan(frameHeight / (focalLength * 2)));
		updateProjectionMatrix();

	}

	void updateProjectionMatrix(){

		T fov = Math::radToDeg<T>(2 * std::atan(std::tan( Math::degToRad<T>(this->fov) * 0.5) / zoom ));

		if (fullWidth != -1){

			T aspect = fullWidth / fullHeight;
			T top = std::tan(Math::degToRad<T>(fov * 0.5) ) * near;
			T bottom = -top;
			T left = aspect * bottom;
			T right = aspect * top;
			T width = std::abs(right - left);
			T height = std::abs(top - bottom);

			this->projectionMatrix.makeFrustum(
					left + x * width / fullWidth,
					left + (x + width) * width / fullWidth,
					top - (y + height) * height / fullHeight,
					top - y * height / fullHeight,
					near,
					far
			);

		} else {

			this->projectionMatrix.makePerspective(fov, aspect, near, far);

		}

	}

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
		return copy(c);
	}

};

#endif
