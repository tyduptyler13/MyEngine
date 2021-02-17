#include "PerspectiveCamera.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;

NBIND_CLASS(View<float>, View) {
	construct<float, float, float, float, float, float>();
	construct<const View<float>&>();
}

NBIND_CLASS(PerspectiveCamera<float>, PerspectiveCamera) {
	inherit(Camera<float>);

	construct<float, float, float, float>();
	construct<const PerspectiveCamera<>&>();
	//TODO getset for the matricies

	getset(getAspect, setAspect);
	getset(getFov, setFov);
	getset(getNear, setNear);
	getset(getFar, setFar);

	method(setViewOffset);
	method(clearViewOffset);
	method(updateProjectionMatrix);

	method(copy);
}
