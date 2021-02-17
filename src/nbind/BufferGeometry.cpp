
#include "BufferGeometry.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;

NBIND_CLASS(BufferGeometry<float>, BufferGeometry) {

	inherit(IGeometry<float>);

	construct<>();
	construct<const BufferGeometry<float>&>();

}
