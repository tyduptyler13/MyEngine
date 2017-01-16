#include "Geometry.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(IGeometry<float>, Geometry){

	method(rotateX);
	method(rotateY);
	method(rotateZ);

	method(translate);

	method(scale);

	method(lookAt);

	method(center);

	multimethod(operator=, args(const IGeometry<float>&), "equals");

	method(computeBoundingBox);
	method(computeBoundingSphere);

}

