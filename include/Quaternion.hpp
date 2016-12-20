#ifndef MYUPLAY_MYENGINE_QUATERNION
#define MYUPLAY_MYENGINE_QUATERNION

#include <vector>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct Quaternion;

		typedef Quaternion<float> Quaternionf;
		typedef Quaternion<double> Quaterniond;

	}
}

#include "Euler.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"

template <typename T=float>
struct MyUPlay::MyEngine::Quaternion {

	T x, y, z, w;

	Quaternion(T x = 0, T y = 0, T z = 0, T w = 1)
	:x(x), y(y), z(z), w(w){}

	Quaternion(const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w){}

	T getX() const {
		return x;
	}

	T getY() const {
		return y;
	}

	T getZ() const {
		return z;
	}

	T getW() const {
		return w;
	}

	void set(T x, T y, T z, T w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void setX(T n) {
		x = n;
	}

	void setY(T n) {
		y = n;
	}

	void setZ(T n) {
		z = n;
	}

	void setW(T n) {
		w = n;
	}

	Quaternion& copy(const Quaternion& q){
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}

	Quaternion& operator=(const Quaternion& q){
		return copy(q);
	}

	Quaternion clone() const {
		return Quaternion(*this);
	}

	Quaternion& setFromEuler(const Euler<T>& euler){
		T c1 = cos(euler.x / 2);
		T c2 = cos(euler.y / 2);
		T c3 = cos(euler.z / 2);
		T s1 = sin(euler.x / 2);
		T s2 = sin(euler.y / 2);
		T s3 = sin(euler.z / 2);

		const Order& order = euler.order;

		if ( order == Order::XYZ ) {

			x = s1 * c2 * c3 + c1 * s2 * s3;
			y = c1 * s2 * c3 - s1 * c2 * s3;
			z = c1 * c2 * s3 + s1 * s2 * c3;
			w = c1 * c2 * c3 - s1 * s2 * s3;

		} else if ( order == Order::YXZ ) {

			x = s1 * c2 * c3 + c1 * s2 * s3;
			y = c1 * s2 * c3 - s1 * c2 * s3;
			z = c1 * c2 * s3 - s1 * s2 * c3;
			w = c1 * c2 * c3 + s1 * s2 * s3;

		} else if ( order == Order::ZXY ) {

			x = s1 * c2 * c3 - c1 * s2 * s3;
			y = c1 * s2 * c3 + s1 * c2 * s3;
			z = c1 * c2 * s3 + s1 * s2 * c3;
			w = c1 * c2 * c3 - s1 * s2 * s3;

		} else if ( order == Order::ZYX ) {

			x = s1 * c2 * c3 - c1 * s2 * s3;
			y = c1 * s2 * c3 + s1 * c2 * s3;
			z = c1 * c2 * s3 - s1 * s2 * c3;
			w = c1 * c2 * c3 + s1 * s2 * s3;

		} else if ( order == Order::YZX ) {

			x = s1 * c2 * c3 + c1 * s2 * s3;
			y = c1 * s2 * c3 + s1 * c2 * s3;
			z = c1 * c2 * s3 - s1 * s2 * c3;
			w = c1 * c2 * c3 - s1 * s2 * s3;

		} else if ( order == Order::XZY ) {

			x = s1 * c2 * c3 - c1 * s2 * s3;
			y = c1 * s2 * c3 - s1 * c2 * s3;
			z = c1 * c2 * s3 + s1 * s2 * c3;
			w = c1 * c2 * c3 + s1 * s2 * s3;

		}

		return *this;

	}

	Quaternion& setFromAxisAngle(const Vector3<T>& axis, T angle){
		T halfAngle = angle / 2;
		T s = sin(halfAngle);

		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		w = cos(halfAngle);

		return *this;

	}

	Quaternion& setFromRotationMatrix(const Matrix4<T>& m) {
		const auto& te = m.elements;
		const T& m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ],
				m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ],
				m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ];

		T trace = m11 + m22 + m33;
		T s;

		if ( trace > 0 ) {

			s = 0.5 / sqrt( trace + 1.0 );

			w = 0.25 / s;
			x = ( m32 - m23 ) * s;
			y = ( m13 - m31 ) * s;
			z = ( m21 - m12 ) * s;

		} else if ( m11 > m22 && m11 > m33 ) {

			s = 2.0 * sqrt( 1.0 + m11 - m22 - m33 );

			w = ( m32 - m23 ) / s;
			x = 0.25 * s;
			y = ( m12 + m21 ) / s;
			z = ( m13 + m31 ) / s;

		} else if ( m22 > m33 ) {

			s = 2.0 * sqrt( 1.0 + m22 - m11 - m33 );

			w = ( m13 - m31 ) / s;
			x = ( m12 + m21 ) / s;
			y = 0.25 * s;
			z = ( m23 + m32 ) / s;

		} else {

			s = 2.0 * sqrt( 1.0 + m33 - m11 - m22 );

			w = ( m21 - m12 ) / s;
			x = ( m13 + m31 ) / s;
			y = ( m23 + m32 ) / s;
			z = 0.25 * s;

		}

		return *this;

	}

	Quaternion& setFromUnitVectors(const Vector3<T>& from, const Vector3<T>& to){

		Vector3<T> v1;
		const T EPS = 0.000001;

		T r = from.dot( to ) + 1;

		if ( r < EPS ) {

			r = 0;

			if ( abs( from.x ) > abs( from.z ) ) {

				v1.set( - from.y, from.x, 0 );

			} else {

				v1.set( 0, - from.z, from.y );

			}

		} else {

			v1.crossVectors( from, to );

		}

		x = v1.x;
		y = v1.y;
		z = v1.z;
		w = r;

		return normalize();

	}

	Quaternion& inverse() {
		conjugate().normalize();

		return *this;
	}

	Quaternion& conjugate(){
		x *= -1;
		y *= -1;
		z *= -1;

		return *this;

	}

	T dot(Quaternion& q) const {
		return x * q.x + q * q.y + z * q.z + w * q.w;
	}

	T lengthSq() const {
		return x * x + y * y + z * z + w * w;
	}

	T length() const {
		return std::sqrt(lengthSq());
	}

	Quaternion& normalize(){

		T len = length();

		if (len == 0){
			x = y = z = 0;
			w = 1;
		} else {
			len = 1 / len;
			x = x * len;
			y = y * len;
			z = z * len;
			w = w * len;
		}

		return *this;
	}

	Quaternion& multiply(const Quaternion& q) {
		return multiplyQuaternions(*this, q);
	}

	Quaternion& multiplyQuaternions(const Quaternion& a, const Quaternion& b) {
		// from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm

		T qax = a.x, qay = a.y, qaz = a.z, qaw = a.w;
		T qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;

		x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
		y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
		z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
		w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

		return *this;
	}

	Quaternion& operator*=(const Quaternion& q){
		return multiply(q);
	}

	Quaternion operator*(const Quaternion& q) const {
		return Quaternion(*this).multiply(q);
	}

	Quaternion& slerp(const Quaternion& qb, T interpolation){
		if ( interpolation == 0 ) return *this;
		if ( interpolation == 1 ) return copy( qb );

		T x = this->x, y = this->y, z = this->z, w = this->w;

		// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

		T cosHalfTheta = w * qb.w + x * qb.x + y * qb.y + z * qb.z;

		if ( cosHalfTheta < 0 ) {

			this->w = - qb.w;
			this->x = - qb.x;
			this->y = - qb.y;
			this->z = - qb.z;

			cosHalfTheta = - cosHalfTheta;

		} else {

			copy( qb );

		}

		if ( cosHalfTheta >= 1.0 ) {

			this->w = w;
			this->x = x;
			this->y = y;
			this->z = z;

			return this;

		}

		T sinHalfTheta = std::sqrt( 1.0 - cosHalfTheta * cosHalfTheta );

		if ( std::abs( sinHalfTheta ) < 0.001 ) {

			this->w = 0.5 * ( w + this->w );
			this->x = 0.5 * ( x + this->x );
			this->y = 0.5 * ( y + this->y );
			this->z = 0.5 * ( z + this->z );

			return this;

		}

		T halfTheta = std::atan2( sinHalfTheta, cosHalfTheta );
		T ratioA = std::sin( ( 1 - interpolation ) * halfTheta ) / sinHalfTheta,
				ratioB = std::sin( interpolation * halfTheta ) / sinHalfTheta;

		this->w = ( w * ratioA + this->w * ratioB );
		this->x = ( x * ratioA + this->x * ratioB );
		this->y = ( y * ratioA + this->y * ratioB );
		this->z = ( z * ratioA + this->z * ratioB );

		return this;
	}

	bool equals(const Quaternion& q) const {
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}

	bool operator==(const Quaternion& q) const {
		return equals(q);
	}

	Quaternion& fromArray(std::vector<Quaternion>& array, unsigned offset = 0) {

		x = array[offset];
		y = array[offset + 1];
		z = array[offset + 2];
		w = array[offset + 3];

		return *this;

	}

	std::vector<Quaternion> toArray(std::vector<Quaternion>& array, unsigned offset = 0) const {

		array.reserve(offset + 3);
		array[offset] = x;
		array[offset + 1] = y;
		array[offset + 2] = z;
		array[offset + 3] = w;

		return array;

	}

	static inline Quaternion& slerp(const Quaternion& a, const Quaternion& b, Quaternion& target, T interpolation){
		return target(a).slerp(b, interpolation);
	}

};

#endif
