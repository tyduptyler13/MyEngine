#include "Vector4.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;
NBIND_CLASS(Vector4f, Vector4) {
	construct<>();
	construct<float, float, float, float>();
	construct<const Vector4f&>();

	getset(getX, setX);
	getset(getY, setY);
	getset(getZ, setZ);
	getset(getW, setW);

	multimethod(set, args(float, float, float, float));

	multimethod(add, args(const Vector4f&));
	multimethod(add, args(float), "addScalar");
	method(addVectors);

	multimethod(sub, args(const Vector4f&));
	multimethod(sub, args(float), "subScalar");
	method(subVectors);

	//multimethod(multiply, args(const Vector4f&));
	multimethod(multiply, args(float), "multiplyScalar");
	//method(multiplyVectors);

	//multimethod(divide, args(const Vector4f&));
	multimethod(divide, args(float), "divideScalar");
	//method(divideVectors); Missing?


	method(min);
	method(max);
	//method(clamp);
	//method(clampScalar);
	method(floor);
	method(ceil);
	method(round);
	//method(roundToZero);
	method(negate);

	method(dot);
	method(lengthSq);
	method(length);
	method(lengthManhattan);
	method(normalize);

	method(setLength);
	method(lerp);
	method(lerpVectors);

	method(equals);
}
