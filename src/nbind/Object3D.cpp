#include "Object3D.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;
NBIND_CLASS(Object3D<float>, Object3D) {
	construct<>();
	construct<Object3D<float>>();

	multimethod(add, args(std::shared_ptr<Object3D<float>>));
	multimethod(remove, args(std::shared_ptr<Object3D<float>>));

	getset(getVisible, setVisible);
	getset(getFrustumCulled, setFrustumCulled);
	getset(getMatrixWorldNeedsUpdate, setMatrixWorldNeedsUpdate);
	getset(getPosition, setPosition);
	getset(getScale, setScale);
	getset(getName, setName);

	getter(getId);
	getter(getUUID);

	method(copy);

	method(updateMatrix);
	method(updateMatrixWorld);

	multimethod(getWorldDirection, args());
	multimethod(getWorldScale, args());

	multimethod(getObjectByName, args(const std::string&));

	method(localToWorld);
	method(worldToLocal);

	method(lookAt);

	method(rotateX);
	method(rotateY);
	method(rotateZ);
	method(rotateOnAxis);

	method(translateX);
	method(translateY);
	method(translateZ);
	method(translateOnAxis);
	method(operator==, "equals");

}
