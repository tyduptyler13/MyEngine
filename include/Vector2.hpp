#ifndef MYUPLAY_MYENGINE_VECTOR2
#define MYUPLAY_MYENGINE_VECTOR2

#include <cmath>
#include <vector>

#include "Math.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct Vector2;

		typedef Vector2<float> Vector2f;
		typedef Vector2<double> Vector2d;
	}
}

/**
 * This class supports any numeric type or class that supports
 * all of the standard operator for floats or doubles
 */
template <typename T = float>
struct MyUPlay::MyEngine::Vector2 {
	T x, y;

	Vector2() : x(0), y(0) {}
	Vector2(T x, T y) : x(x), y(y) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}

	Vector2& set(T x, T y){
		this->x = x;
		this->y = y;
		return *this;
	}

	Vector2& set(T s){
		this->x = s;
		this->y = s;
		return *this;
	}

	T getX() const {
		return x;
	}

	Vector2& setX(T x){
		this->x = x;
		return *this;
	}

	T getY() const {
		return y;
	}

	Vector2& setY(T y){
		this->y = y;
		return *this;
	}

	Vector2& copy(const Vector2& v){
		x = v.x;
		y = v.y;
		return *this;
	}

	Vector2& add(const Vector2& v){
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2& addVectors(const Vector2& a, const Vector2& b){
		x = a.x + b.x;
		y = a.y + a.y;
		return *this;
	}

	Vector2& add(T scalar){
		x += scalar;
		y += scalar;
		return *this;
	}

	Vector2& sub(const Vector2& v){
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2& subVectors(const Vector2& a, const Vector2& b){
		x = a.x - b.x;
		y = a.y - b.y;
		return *this;
	}

	Vector2& sub(T scalar){
		x -= scalar;
		y -= scalar;
		return *this;
	}

	Vector2& multiply(const Vector2& v){
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2& multiply(T scalar){
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& divideScalar(T scalar){
		if (scalar == 0.0){
			x = 0;
			y = 0;
		}
		else {
			x /= scalar;
			y /= scalar;
		}
		return *this;
	}

	Vector2& negate(){
		x = -x;
		y = -y;
		return *this;
	}

	Vector2& floor(){

		x = std::floor(x);
		y = std::floor(y);

		return *this;

	}

	Vector2& ceil(){

		x = std::ceil(x);
		y = std::ceil(y);

		return *this;

	}

	Vector2& round(){

		x = std::round(x);
		y = std::round(y);

		return *this;

	}

	Vector2& roundToZero() {

		x = x < 0 ? std::ceil(x) : std::floor(x);
		y = y < 0 ? std::ceil(y) : std::floor(y);

		return *this;

	}

	inline Vector2 operator-() const {
		return Vector2(*this).negate();
	}

	Vector2 operator+(const T scalar) const {
		return Vector2(*this).add(scalar);
	}

	inline Vector2& operator=(const Vector2& v){
		return copy(v);
	}

	inline Vector2& operator+=(const Vector2& v){
		return add(v);
	}

	inline Vector2& operator +=(T scalar){
		return add(scalar);
	}

	Vector2 operator+(const Vector2& b) const{
		Vector2 v;
		v.x = x + b.x;
		v.y = y + b.y;
		return v;
	}

	inline Vector2& operator-=(const Vector2& v){
		return sub(v);
	}

	inline Vector2& operator-=(T scalar){
		return sub(scalar);
	}

	Vector2 operator-(const Vector2& v) const {
		return Vector2(*this).sub(v);
	}

	Vector2 operator-(T scalar) const {
		return Vector2(*this).sub(scalar);
	}

	inline Vector2 operator*=(T s) const {
		return Vector2(*this).multiply(s);
	}

	inline Vector2 operator*=(const Vector2& v) const {
		return Vector2(*this).multiply(v);
	}

	Vector2 operator*(T s) const{
		Vector2 v;
		v.x = x * s;
		v.y = y * s;
		return v;
	}

	Vector2 operator*(const Vector2& b) const{
		Vector2 v;
		v.x = x * b.x;
		v.y = y * b.y;
		return v;
	}

	Vector2& divide(T scalar){
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline Vector2& operator/=(const Vector2& v){
		return divide(v);
	}

	inline Vector2& operator/=(T scalar){
		return divide(scalar);
	}

	Vector2 operator/(const Vector2& v) const{
		Vector2 b;
		b.x = x / v.x;
		b.y = y / v.y;
		return b;
	}

	Vector2 operator/(T s) const{
		Vector2 v;
		v.x = x / s;
		v.y = y / s;
		return v;
	}


	Vector2& min(const Vector2& v){

		if (x > v.x){
			x = v.x;
		}

		if (y > v.y){
			y = v.y;
		}

		return *this;

	}

	Vector2& max(const Vector2& v){

		if (x < v.x){
			x = v.x;
		}

		if (y < v.y){
			y = v.y;
		}

		return *this;

	}

	Vector2& clamp(const Vector2& min, const Vector2& max){

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

	Vector2& clampScalar(T min, T max){

		Vector2 low(min, min), high(max, max);

		return clamp(low, high);

	}

	T length() const {

		return sqrt(x * x + y * y);

	}
	T lengthSq() const {
		return x * x + y * y;
	}
	T lengthManhattan() const {
		return std::abs(x) + std::abs(y);
	}

	Vector2& normalize(){
		return divideScalar(length());
	}

	T angle() const {
		T angle = atan2(y, x);
		if (angle < 0) angle += 2 * Math::PI;
		return angle;
	}

	T distanceTo(const Vector2& v) const {
		return sqrt(distanceToSquared(v));
	}
	T distanceToSquared(const Vector2& v) const {
		T dx = x - v.x, dy = y - v.y;
		return dx * dx + dy * dy;
	}

	Vector2& setLength(T length){
		return multiply(length / this->length());
	}

	Vector2& lerp(const Vector2& v, T alpha){
		x += (v.x - x) * alpha;
		y += (v.y - y) * alpha;
		return *this;
	}

	Vector2& lerpVectors(const Vector2& v1, const Vector2& v2, T alpha){
		return subVectors(v2, v1).multiply(alpha).add(v1);
	}

	Vector2& rotateAround(const Vector2& center, T angle){

		T c = cos(angle), s = sin(angle);

		T x = this->x - center.x;
		T y = this->y - center.y;

		this->x = x * c - y * s + center.x;
		this->y = x * s + y * c + center.y;

		return *this;

	}

	T dot(const Vector2& v) const {
		return x * v.x + y * v.y;
	}

	bool equals(const Vector2& v) const {
		return x == v.x && y == v.y;
	}

	bool operator==(const Vector2& v) const {
		return equals(v);
	}

	bool operator!=(const Vector2& v) const {
		return !equals(v);
	}

	void toArray(std::vector<T>& array, unsigned offset) const {
		array[offset] = x;
		array[offset + 1] = y;
	}

	Vector2& fromArray(const std::vector<T>& array, unsigned offset){
		x = array[offset];
		y = array[offset + 1];
		return *this;
	}

};

#endif
