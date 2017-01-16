
#include "Geometries/BoxGeometry.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(BoxGeometry<float>, BoxGeometry) {

	inherit(BufferGeometry<float>);

	construct<float, float, float>();
	construct<float, float, float, unsigned, unsigned, unsigned>();

}
