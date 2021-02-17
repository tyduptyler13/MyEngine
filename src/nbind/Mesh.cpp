
#include "Mesh.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;

NBIND_CLASS(Mesh<>, Mesh) {

	inherit(Object3D<float>);

	construct<std::shared_ptr<IGeometry<float>>, std::shared_ptr<IMaterial>>();
	construct<const Mesh<>&>();

	getter(getGeometry);
	getter(getMaterial);

}
