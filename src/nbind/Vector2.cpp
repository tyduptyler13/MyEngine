#include "Vector2.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;
NBIND_CLASS(Vector2f, Vector2) {
	construct<>();
	construct<float, float>();
	construct<Vector2f>();

	getset(getX, setX);
	getset(getY, setY);

	multimethod(set, args(float, float));

	multimethod(add, args(const Vector2f&));
	multimethod(add, args(float), "addScalar");
	method(addVectors);

	multimethod(sub, args(const Vector2f&));
	multimethod(sub, args(float), "subScalar");
	method(subVectors);

	multimethod(multiply, args(const Vector2f&));
	multimethod(multiply, args(float), "multiplyScalar");
	//method(multiplyVectors);

	//multimethod(divide, args(const Vector2f&));
	multimethod(divide, args(float), "divideScalar");
	//method(divideVectors); Missing?

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

	method(distanceTo);
	method(distanceToSquared);

	//method(setFromMatrixPosition);
	//method(setFromMatrixScale);
	//method(setFromMatrixColumn);

	method(equals);
	//toArray and fromArray???
}
