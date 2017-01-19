
#include "BufferGeometry.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(BufferGeometry<float>, BufferGeometry) {

	inherit(IGeometry<float>);

	construct<>();

}
