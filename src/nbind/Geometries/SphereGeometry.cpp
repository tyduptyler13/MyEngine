#include "Geometries/SphereGeometry.hpp"
#include "nbind/nbind.h"

using namespace MyEngine;

NBIND_CLASS(SphereGeometry<float>, SphereGeometry) {

	inherit(BufferGeometry<float>);

	construct<float, unsigned, unsigned, float, float, float, float>();
	construct<float, unsigned, unsigned>();
	construct<float>();
	//construct<const SphereGeometry<float>&>();

}
