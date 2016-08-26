#ifndef MYUPLAY_MYENGINE_VECTOR4
#define MYUPLAY_MYENGINE_VECTOR4

#include <limits>
#include <cmath>

#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "Math.hpp"

namespace MyUPlay {

	namespace MyEngine {

		/**
		 * While this is templated, it is highly recommended to use float types.
		 * Integers will have strange behavior in some of the functions.
		 */
		template <typename T>
		class Vector4 {

		public:
			T x = 0, y = 0, z = 0, w = 1;

			Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
			Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

			Vector4& set(T x, T y, T z, T w) {
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
				return *this;
			}

			Vector4& set(T s){
				x = y = z = w = s;
				return *this;
			}

			Vector4& operator=(T s) {
				return set(s);
			}

			Vector4& setX(T s){
				x = s;
				return *this;
			}

			Vector4& setY(T s){
				y = s;
				return *this;
			}

			Vector4& setZ(T s){
				z = s;
				return *this;
			}

			Vector4& setW(T s){
				w = s;
				return *this;
			}

			Vector4& operator=(const Vector4& v){
				return set(v.x, v.y, v.z, v.w);
			}

			Vector4& add(const Vector4& v){
				x += v.x;
				y += v.y;
				z += v.z;
				w += v.w;
				return *this;
			}

			Vector4& operator+=(const Vector4& v){
				return add(v);
			}

			Vector4& add(T s){
				x += s;
				y += s;
				z += s;
				w += s;
				return *this;
			}

			Vector4& operator+=(T s){
				return add(s);
			}

			Vector4 operator+(const Vector4& b) const {
				Vector4 v(*this);
				v.add(b);
				return v;
			}

			Vector4 operator+(T s) const {
				Vector4 v(*this);
				v.add(s);
				return v;
			}

			Vector4& sub(const Vector4& v){
				x -= v.x;
				y -= v.y;
				z -= v.z;
				w -= v.w;
				return *this;
			}

			Vector4& operator-=(const Vector4& v){
				return sub(v);
			}

			Vector4& sub(T s){
				x -= s;
				y -= s;
				z -= s;
				w -= s;
				return *this;
			}

			Vector4& operator-=(T s){
				return sub(s);
			}

			Vector4 operator-(const Vector4& b) const {
				Vector4 v(*this);
				v.sub(b);
				return v;
			}

			Vector4 operator-(T s) const {
				Vector4 v(*this);
				v.sub(s);
				return v;
			}

			Vector4& multiply(T s){
				if (std::numeric_limits<T>::infinity() != s){ //If int, then infinity = 0, which is ok.
					x *= s;
					y *= s;
					z *= s;
					w *= s;
				} else {
					x = y = z = w = 0;
				}
				return *this;
			}

			Vector4& operator*=(T s){
				return multiply(s);
			}

			Vector4& applyMatrix4(const Matrix4<T>& m){
				T x = this->x, y = this->y, z = this->z, w = this->w;
				const auto& e = m.elements;

				this->x = e[0] * x + e[ 4 ] * y + e[ 8 ] * z + e[ 12 ] * w;
				this->y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z + e[ 13 ] * w;
				this->z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ] * w;
				this->w = e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] * w;

				return *this;

			}

			Vector4& divide(T s){
				return multiply(1/s);
			}

			Vector4& operator/=(T s){
				return divide(s);
			}

			Vector4& setAxisAngleFromQuaternion(const Quaternion<T>& q){
				w = 2 * acos(q.w);

				T s = sqrt(1 - q.w * q.w);

				if (s < 0.0001){
					x = 1;
					y = 0;
					z = 0;
				} else {
					x = q.x / s;
					y = q.y / s;
					z = q.z / s;
				}

				return *this;
			}

			Vector4& setAxisAngleFromRotationMatrix(const Matrix4<T>& m) {

				// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm

				// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

				const T angle = acos(-1); //PI
				T x, y, z; // variables for result
				const T epsilon = 0.01, // margin to allow for rounding errors
								epsilon2 = 0.1; // margin to distinguish between 0 and 180 degrees

				const auto& te = m.elements;

				const T& m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ],
				m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ],
				m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ];

				if ((std::abs(m12 - m21) < epsilon) &&
						(std::abs(m13 - m31) < epsilon) &&
						(std::abs(m23 - m32) < epsilon)) {

					// singularity found
					// first check for identity matrix which must have +1 for all terms
					// in leading diagonal and zero in other terms

					if ((std::abs(m12 + m21) < epsilon2) &&
							(std::abs(m13 + m31) < epsilon2) &&
							(std::abs(m23 + m32) < epsilon2) &&
							(std::abs(m11 + m22 + m33 - 3) < epsilon2)){
						// this singularity is identity matrix so angle = 0
						set(1, 0, 0, 0);
						return *this; // zero angle, arbitrary axis
					}

					// otherwise this singularity is angle = 180

					const T xx = (m11 + 1) / 2;
					const T yy = (m22 + 1) / 2;
					const T zz = (m33 + 1) / 2;
					const T xy = (m12 + m21) / 4;
					const T xz = (m13 + m31) / 4;
					const T yz = (m23 + m32) / 4;

					if ((xx > yy) && (xx > zz)){
						// m11 is the largest diagonal term
						if (xx < epsilon){
							x = 0;
							y = 0.707106781;
							z = 0.707106781;
						} else {
							x = std::sqrt(xx);
							y = xy / x;
							z = xz / x;
						}
					} else if (yy > zz){
						// m22 is the largest diagonal term
						if (yy < epsilon){
							x = 0.707106781;
							y = 0;
							z = 0.707106781;
						} else {
							y = std::sqrt(yy);
							x = xy / y;
							z = yz / y;
						}
					} else {
						// m33 is the largest diagonal term so base result on this
						if (zz < epsilon){
							x = 0.707106781;
							y = 0.707106781;
							z = 0;
						} else {
							z = std::sqrt(zz);
							x = xz / z;
							y = yz / z;
						}
					}

					set(x, y, z, angle);

					return *this;

				}

				// as we have reached here there are no singularities so we can handle normally

				T s = std::sqrt(( m32 - m23 ) * ( m32 - m23 ) +
		                   ( m13 - m31 ) * ( m13 - m31 ) +
		                   ( m21 - m12 ) * ( m21 - m12 )); //used to normalize

				if (std::abs(s) < 0.001) s = 1;

				// prevent divide by zero, should not happen if matrix is orthogonal and should be
				// caught by singularity test above, but I've left it in just in case

				this->x = (m32 - m23) / s;
				this->y = (m13 - m31) / s;
				this->z = (m21 - m12) / s;
				this->w = acos((m11 + m22 + m33 - 1) / 2);

				return *this;

			}

			Vector4& min(const Vector4& v){
				x = std::min(x, v.x);
				y = std::min(y, v.y);
				z = std::min(z, v.z);
				w = std::min(w, v.w);
				return *this;
			}

			Vector4& max(const Vector4& v){
				x = std::max(x, v.x);
				y = std::max(y, v.y);
				z = std::max(z, v.z);
				w = std::max(w, v.w);
				return *this;
			}

			Vector4& clamp(const Vector4& min, const Vector4& max){
				x = Math::clamp<T>(x, min.x, max.x);
				y = Math::clamp<T>(y, min.y, max.y);
				z = Math::clamp<T>(z, min.z, max.z);
				w = Math::clamp<T>(w, min.w, max.w);
				return *this;
			}

			Vector4& clamp(T min, T max){
				x = Math::clamp<T>(x, min, max);
				y = Math::clamp<T>(y, min, max);
				z = Math::clamp<T>(z, min, max);
				w = Math::clamp<T>(w, min, max);
				return *this;
			}

			Vector4& floor(){
				x = std::floor(x);
				y = std::floor(y);
				z = std::floor(y);
				w = std::floor(w);
				return *this;
			}

			Vector4& ceil(){
				x = std::ceil(x);
				y = std::ceil(y);
				z = std::ceil(z);
				w = std::ceil(w);
				return *this;
			}

			Vector4& round(){ //Rounds to zero
				x = std::round(x);
				y = std::round(y);
				z = std::round(z);
				w = std::round(w);
				return *this;
			}

			Vector4& negate(){
				x = -x;
				y = -y;
				z = -z;
				w = -w;
				return *this;
			}

			Vector4 operator-(){
				Vector4 v(*this);
				v.negate();
				return v;
			}

			T dot(const Vector4& v) const {
				return x * v.x + y * v.y + z * v.z + w * v.w;
			}

			T lengthSq() const {
				return x * x + y * y + z * z + w * w;
			}

			T length() const {
				return sqrt(lengthSq());
			}

			T lengthManhattan() const {
				return std::abs(x) + std::abs(y) + std::abs(z) + std::abs(w);
			}

			Vector4& normalize(){
				return divideScalar(length());
			}

			Vector4& setLength(T length){
				return multiplyScalar(length / this->length());
			}

			Vector4& lerp(const Vector4& v, T alpha){
				x += (v.x - x) * alpha;
				y += (v.y - y) * alpha;
				z += (v.z - z) * alpha;
				w += (v.w - w) * alpha;
				return *this;
			}

			Vector4& lerpVectors(const Vector4& v1, const Vector4& v2, T alpha){
				return set(v1 - v2 * alpha + v1);
			}

			bool equals(const Vector4& v) const {
				return x == v.x && y == v.y && z == v.z && w == v.w;
			}

			bool operator==(const Vector4& v) const {
				return equals(v);
			}

		};

	}

}

#endif
