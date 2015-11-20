#include "PerspectiveCamera.hpp"
#include "Math.hpp"

#include <cmath>

using namespace MyUPlay::MyEngine;

void PerspectiveCamera::setLens(float focalLength, float frameHeight) {

	fov = 2 * Math::radToDeg<float>(std::atan(frameHeight / (focalLength * 2)));
	updateProjectionMatrix();

}

void PerspectiveCamera::updateProjectionMatrix(){

	float fov = Math::radToDeg<float>(2 * std::atan(std::tan( Math::degToRad<float>(this->fov) * 0.5) / zoom ));

	if (fullWidth != -1){

		float aspect = fullWidth / fullHeight;
		float top = std::tan(Math::degToRad<float>(fov * 0.5) ) * near;
		float bottom = -top;
		float left = aspect * bottom;
		float right = aspect * top;
		float width = std::abs(right - left);
		float height = std::abs(top - bottom);

		projectionMatrix.makeFrustrum(
				left + x * width / fullWidth,
				left + (x + width) * width / fullWidth,
				top - (y + height) * height / fullHeight,
				top - y * height / fullHeight,
				near,
				far
		);

	} else {

		projectionMatrix.makePerspective(fov, aspect, near, far);

	}

}

