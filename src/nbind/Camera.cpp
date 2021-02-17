#include "Camera.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;

NBIND_CLASS(Camera<float>, Camera) {
	inherit(Object3D<float>);
	//TODO getset for the matricies

	method(lookAt);
	//TODO method(getWorldDirection);
	method(updateProjectionMatrix);

}
