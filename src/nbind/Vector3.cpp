#include "Vector3.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;
NBIND_CLASS(Vector3f, Vector3) {
	construct<>();
	construct<float, float, float>();
	construct<Vector3f>();

	getset(getX, setX);
	getset(getY, setY);
	getset(getZ, setZ);

	method(set);

	multimethod(add, args(const Vector3f&));
	multimethod(add, args(float), "addScalar");
	method(addVectors);

	multimethod(sub, args(const Vector3f&));
	multimethod(sub, args(float), "subScalar");
	method(subVectors);

	multimethod(multiply, args(const Vector3f&));
	multimethod(multiply, args(float), "multiplyScalar");
	method(multiplyVectors);

	multimethod(divide, args(const Vector3f&));
	multimethod(divide, args(float), "divideScalar");
	//method(divideVectors); Missing?

	//method(applyEuler);
	method(applyAxisAngle);
	//method(applyMatrix3);
	//method(applyMatrix4);
	//method(applyProjection);
	//method(applyQuaternion);
	//method(project);
	//method(unproject);
	//method(transformDirection);

	method(min);
	method(max);
	method(clamp);
	method(clampScalar);
	method(floor);
	method(ceil);
	method(round);
	method(roundToZero);
	method(negate);

	method(dot);
	method(lengthSq);
	method(length);
	method(lengthManhattan);
	method(normalize);

	method(setLength);
	method(lerp);
	method(lerpVectors);
	multimethod(cross, args(const Vector3f&));
	multimethod(cross, args(const Vector3f&, const Vector3f&), "crossVectors");

	method(projectOnVector);
	method(projectOnPlane);
	method(reflect);
	method(angleTo);
	method(distanceTo);
	method(distanceToSquared);

	//method(setFromMatrixPosition);
	//method(setFromMatrixScale);
	//method(setFromMatrixColumn);

	method(equals);
	//toArray and fromArray???
}
