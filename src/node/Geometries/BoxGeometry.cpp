
#include "Geometries/BoxGeometry.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

//typedef AGeometry<float, IGeometry<float>> DefaultAGeometry;

NBIND_CLASS(BoxGeometry<float>, BoxGeometry) {

	//inherit(IGeometry<float>);
	//inherit(DefaultAGeometry);

	construct<float, float, float>();
	construct<float, float, float, unsigned, unsigned, unsigned>();

}
