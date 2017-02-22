#include "Geometries/SphereGeometry.hpp"
#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(SphereGeometry<float>, SphereGeometry) {

	inherit(BufferGeometry<float>);

	construct<float, unsigned, unsigned, float, float, float, float>();
	construct<const SphereGeometry<float>&>();

}
