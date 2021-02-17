#pragma once

#include "Math.hpp"


namespace MyEngine {
	template<typename T>
	struct PerspectiveCamera;
	template<typename T>
	struct View;
}


#include "Math.hpp"
#include "Camera.hpp"

template<typename T>
struct MyEngine::View {
	T fullWidth;
	T fullHeight;
	T x;
	T y;
	T width;
	T height;

	View(T fullWidth, T fullHeight, T x, T y, T width, T height) :
			fullWidth(fullWidth), fullHeight(fullHeight), x(x), y(y), width(width), height(height) {}

	View(const View& v) :
			fullWidth(v.fullWidth), fullHeight(v.fullHeight), x(v.x), y(v.y), width(v.width), height(v.height) {}

};

template<typename T = float>
struct MyEngine::PerspectiveCamera : public Camera<T> {

	T zoom = 1;

	T fov = 50;
	T aspect = 1;
	T near = 0.1;
	T far = 2000;

	T focus = 10;

	T filmGauge = 35;
	T filmOffset = 0;

	std::unique_ptr<View < T>> view;

	PerspectiveCamera(T fov = 50, T aspect = 1, T near = 0.1, T far = 2000) : Camera<T>(),
	                                                                          fov(fov), aspect(aspect), near(near),
	                                                                          far(far) {
		updateProjectionMatrix();
	}

	PerspectiveCamera(const PerspectiveCamera& c) : Camera<T>(c) {

		zoom = c.zoom;

		fov = c.fov;
		aspect = c.aspect;
		near = c.near;
		far = c.far;

		focus = c.focus;

		filmGauge = c.filmGauge;
		filmOffset = c.filmOffset;

		*view = *c.view;

	}

	/**
	 * Sets the FOV by focal length in respect to the current .filmGauge.
	 *
	 * The default film gauge is 35, so that the focal length can be specified for
	 * a 35mm (full frame) camera.
	 *
	 * Values for focal length and film gauge must have the same unit.
	 */
	void setFocalLength(T focalLength) {

		// see http://www.bobatkins.com/photography/technical/field_of_view.html
		T vExtentSlope = 0.5 * getFilmHeight() / focalLength;

		fov = Math::radToDeg<T>(2 * std::atan(vExtentSlope));
		updateProjectionMatrix();
	}

	T getFocalLength() {
		T vExtentSlope = std::tan(Math::degToRad<T>(0.5 * fov));

		return 0.5 * getFilmHeight() / vExtentSlope;
	}

	T getEffectiveFov() {
		return Math::radToDeg<T>(2 * std::atan(std::tan(Math::degToRad<T>(0.5 * fov)) / zoom));
	}

	T getFilmWidth() {
		return filmGauge * std::min<T>(aspect, 1.0);
	}

	T getFilmHeight() {
		return filmGauge / std::max<T>(aspect, 1.0);
	}

	void setViewOffset(T fullWidth, T fullHeight, T x, T y, T width, T height) {

		aspect = fullWidth / fullHeight;

		view = std::make_unique<View<T>>(fullWidth, fullHeight, x, y, width, height);

		updateProjectionMatrix();

	}

	void clearViewOffset() {
		view = nullptr;
		updateProjectionMatrix();
	}

	void updateProjectionMatrix() {

		T near = this->near,
				top = near * std::tan(Math::degToRad<T>(0.5 * fov)) / zoom,
				height = 2 * top,
				width = aspect * height,
				left = -0.5 * width;

		if (view != nullptr) {

			T& fullWidth = view->fullWidth,
					fullHeight = view->fullHeight;

			left += view->x * width / fullWidth;
			top -= view->y * height / fullHeight;
			width *= view->width / fullWidth;
			height *= view->height / fullHeight;

		}

		T skew = filmOffset;
		if (skew != 0) left += near * skew / getFilmWidth();

		this->projectionMatrix.makePerspective(left, left + width, top, top - height, near, far);

	}

	PerspectiveCamera& copy(const PerspectiveCamera& c) {

		Camera<T>::copy(c);

		fov = c.fov;
		aspect = c.fov;
		near = c.near;
		far = c.far;

		zoom = c.zoom;

		focus = c.focus;

		filmGauge = c.filmGauge;
		filmOffset = c.filmOffset;

		*view = *c.view;

		return *this;

	}

	inline PerspectiveCamera& operator=(const PerspectiveCamera& c) {
		return copy(c);
	}

	T getAspect() {
		return aspect;
	}

	void setAspect(T v) {
		aspect = v;
	}

	T getFov() {
		return fov;
	}

	void setFov(T v) {
		fov = v;
	}

	T getNear() {
		return near;
	}

	void setNear(T v) {
		near = v;
	}

	T getFar() {
		return far;
	}

	void setFar(T v) {
		far = v;
	}

};

