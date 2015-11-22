
#include "Vector3.hpp"
#include "Math.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

template <typename T>
Vector3<T>& Vector3<T>::applyMatrix3(const Matrix3<T>& m){

	T x = this->x, y = this->y, z = this->z;

	T e[] = m.elements;

	this->x = e[0] * x + e[3] * y + e[6] * z;
	this->y = e[1] * x + e[4] * y + e[7] * z;
	this->z = e[2] * x + e[5] * y + e[8] * z;

	return *this;

}

template <typename T>
Vector3<T>& Vector3<T>::applyMatrix4(const Matrix4<T>& m){

	T x = this->x, y = this->y, z = this->z;

	T e[] = m.elements;

	this->x = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ]  * z + e[ 12 ];
	this->y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ]  * z + e[ 13 ];
	this->z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ];

	return *this;	

}

template <typename T>
Vector3<T>& Vector3<T>::applyProjection(const Matrix4<T>& m){

	T x = this->x, y = this->y, z = this->z;

	T e[] = m.elements;
	T d = 1 / ( e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] ); // perspective divide

	this->x = ( e[ 0 ] * x + e[ 4 ] * y + e[ 8 ]  * z + e[ 12 ] ) * d;
	this->y = ( e[ 1 ] * x + e[ 5 ] * y + e[ 9 ]  * z + e[ 13 ] ) * d;
	this->z = ( e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ] ) * d;

	return *this;

}

template <typename T>
Vector3<T>& Vector3<T>::applyQuaternion(const Quaternion<T>& q){

	T x = this->x, y = this->y, z = this->z;

	T qx = q.x;
	T qy = q.y;
	T qz = q.z;
	T qw = q.w;

	// calculate quat * vector

	T ix =  qw * x + qy * z - qz * y;
	T iy =  qw * y + qz * x - qx * z;
	T iz =  qw * z + qx * y - qy * x;
	T iw = - qx * x - qy * y - qz * z;

	// calculate result * inverse quat

	this->x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
	this->y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
	this->z = iz * qw + iw * - qz + ix * - qy - iy * - qx;

	return this;	

}

template <typename T>
Vector3<T>& Vector3<T>::project(const Camera<T>& camera){

	Matrix4<T> matrix;

	matrix.multiplyMatricies(camera.matrixWorld, matrix.getInverse( camera.projectionMatrix ) );
	return applyProjection();

}

template <typename T>
Vector3<T>& Vector3<T>::unproject(const Camera<T>& camera){

	Matrix4<T> matrix;

	matrix.multiplyMatricies(camera.matrixWorld, matrix.getInverse( camera.projectionMatrix ) );
	return applyProjection(matrix);

}

template <typename T>
Vector3<T>& Vector3<T>::transformDirection(const Matrix4<T>& m) {

	T x = this->x, y = this->y, z = this->z;

	T e[] = m.elements;

	this->x = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ]  * z;
	this->y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ]  * z;
	this->z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z;

	normalize();

	return *this;

}

template <typename T>
Vector3<T>& Vector3<T>::min(const Vector3<T>& v){

	if (x > v.x){
		x = v.x;
	}

	if (y > v.y){
		x = v.y;
	}

	if (z > v.z){
		z = v.z;
	}

	return *this;

}

template <typename T>
Vector3<T>& Vector3<T>::max(const Vector3<T>& v){

	if (x < v.x){
		x = v.x;
	}

	if (y < v.y){
		y = v.y;
	}

	if (z < v.z){
		z = v.z;
	}

	return *this;

}

template <typename T>
Vector3<T>& Vector3<T>::clamp(const Vector3<T>& min, const Vector3<T>& max){

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

	if (z < min.z) {
		z = min.z;
	} else if (z > max.z) {
		z = max.z;
	}

	return *this;

}


template <typename T>
Vector3<T>& Vector3<T>::clampScalar(T min, T max) {

	Vector3 low(min, min, min), high(max, max, max);

	return clamp(low, high);

}

template <typename T>
Vector3<T>& Vector3<T>::projectOnVector(const Vector3<T>& vector){

	Vector3 v1(vector);

	v1.normalize();

	T dot = dot(v1);

	return copy(v1).multiply(dot);

}

template <typename T>
Vector3<T>& Vector3<T>::projectOnPlane(const Vector3<T>& normal){

	Vector3 v1(*this);

	v1.projectOnVector(normal);

	return sub(v1);

}

template <typename T>
Vector3<T>& Vector3<T>::reflect(const Vector3<T>& normal) {

	Vector3 v1(normal);

	return sub(v1.multiplyScalar(2 * dot(normal)));

}

template <typename T>
T Vector3<T>::angleTo(const Vector3<T>& v) const {

	T theta = dot(v) / (length() * v.length());

	return acos(Math::clamp(theta, -1, 1));

}

