
#include "Vector2.hpp"
#include "Math.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

template <typename T>
Vector2<T>& Vector2<T>::min(const Vector2<T>& v){

	if (x > v.x){
		x = v.x;
	}

	if (y > v.y){
		x = v.y;
	}

	return *this;

}

template <typename T>
Vector2<T>& Vector2<T>::max(const Vector2<T>& v){

	if (x < v.x){
		x = v.x;
	}

	if (y < v.y){
		y = v.y;
	}

	return *this;

}


template <typename T>
Vector2<T>& Vector2<T>::clamp(const Vector2<T>& min, const Vector2<T>& max){

	if (x < min.x) {
		x = min.x;
	} else if (x > max.x) {
		x = max.x;
	}

	if (y < min.y) {
		y = min.y;
	} else if (y > max.y) {
		y = max.y;
	}
	return *this;

}


template <typename T>
Vector2<T>& Vector2<T>::clampScalar(T min, T max) {

	Vector2 low(min, min), high(max, max);

	return clamp(low, high);

}

template <typename T>
T Vector2<T>::length() const {

	return sqrt(x * x + y * y);

}

template <typename T>
T Vector2<T>::lengthManhattan() const {
	return abs(x) + abs(y);
}

template <typename T>
Vector2<T>& Vector2<T>::normalize(){
	return divideScalar(length());
}

template <typename T>
T Vector2<T>::angle() const {	
	T angle = atan2(y, x);
	if (angle < 0) angle += 2 * Math::PI;
	return angle;
}

template <typename T>
T Vector2<T>::distanceTo(const Vector2& v) const {
	return sqrt(distanceToSquared(v));
}

template <typename T>
Vector2<T>& Vector2<T>::rotateAround(const Vector2& center, T angle) {

	T c = cos(angle), s = sin(angle);

	T x = this->x - center.x;
	T y = this->y - center.y;

	this->x = x * c - y * s + center.x;
	this->y = x * s + y * c + center.y;

	return *this;

}

