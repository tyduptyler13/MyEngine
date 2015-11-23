#include "PerspectiveCamera.hpp"
#include "Math.hpp"

#include <cmath>

using namespace MyUPlay::MyEngine;

template <typename T>
void PerspectiveCamera<T>::setLens(T focalLength, T frameHeight) {

	fov = 2 * Math::radToDeg<T>(std::atan(frameHeight / (focalLength * 2)));
	updateProjectionMatrix();

}

template <typename T>
void PerspectiveCamera<T>::updateProjectionMatrix(){

	T fov = Math::radToDeg<T>(2 * std::atan(std::tan( Math::degToRad<T>(this->fov) * 0.5) / zoom ));

	if (fullWidth != -1){

		T aspect = fullWidth / fullHeight;
		T top = std::tan(Math::degToRad<T>(fov * 0.5) ) * near;
		T bottom = -top;
		T left = aspect * bottom;
		T right = aspect * top;
		T width = std::abs(right - left);
		T height = std::abs(top - bottom);

		this->projectionMatrix.makeFrustrum(
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

