
#include "Scene.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(Scene<>, Scene) {
	inherit(Object3D<float>);

	construct<>();
	construct<const Scene<>&>();

	method(copy);

	getset(getAutoUpdate, setAutoUpdate);

}
