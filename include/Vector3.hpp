#ifndef MYUPLAY_MYENGINE_VECTOR3
#define MYUPLAY_MYENGINE_VECTOR3

#include <cmath>

#include "Matrix3.hpp"
#include "Matrix4.hpp"
#include "Quanternion.hpp"
#include "Euler.hpp"
#include "Camera.hpp"

namespace MyUPlay {

	namespace MyEngine {

		/**
		 * This class supports any numeric type or class that supports
		 * all of the standard opperators for floats or doubles.
		 */
		template <typename T = float>
		class Vector3 {

		public:

			T x, y, z;

			Vector3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

			Vector3(const Vector3& v){
				//TODO
			}

			Vector3& set(T x, T y, T z){
				this->x = x;
				this->y = y;
				this->z = z;
				return *this;
			}

			inline Vector3& operator=(const Vector3& v){
				return copy(v);
			}

			Vector3& setX(T x){
				this->x = x;
				return *this;
			}

			Vector3& setY(T y){
				this->y = y;
				return *this;
			}

			Vector3& setZ(T z){
				this->z = z;
				return *this;
			}

			//Removed set and get component

			Vector3& copy(const Vector3& v){
				x = v.x;
				y = v.y;
				z = v.z;

				return *this;
			}

			Vector3& add(const Vector3& v){
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}

			inline Vector3& operator+=(const Vector3& v){
				return add(v);
			}

			Vector3& add(T scalar){
				x += scalar;
				y += scalar;
				z += scalar;
				return *this;
			}

			inline Vector3& operator+=(T scalar){
				return add(scalar);
			}

			Vector3& addVectors(const Vector3& a, const Vector3& b){
				x = a.x + b.x;
				y = a.y + b.y;
				z = a.z + b.z;
				
				return *this;
			}

			Vector3 operator+(const Vector3& b){
				Vector3 v;
				v.x = x + b.x;
				v.y = y + b.y;
				v.z = z + b.z;
				return v;
			}

			inline Vector3 operator+(T s){
				return Vector3(x + s, y + s, z + s);
			}

			Vector3& sub(const Vector3& v){
				x -= v.x;
				y -= v.y;
				z -= v.z;
				
				return *this;
			}

			inline Vector3& operator-=(const Vector3& v){
				return sub(v);
			}

			Vector3& sub(T scalar){
				x -= scalar;
				y -= scalar;
				z -= scalar;
				return *this;
			}

			inline Vector3& operator-=(T scalar){
				return sub(scalar);
			}

			Vector3& subVectors(const Vector3& a, const Vector3& b){
				x = a.x - b.x;
				y = a.y - b.y;
				z = a.z - b.z;
				return *this;
			}

			Vector3 operator-(T s){
				Vector3 v;
				v.x = x * s;
				v.y = y * s;
				v.z = z * s;
				return v;
			}

			Vector3 operator-(const Vector3& b){
				Vector3 v;
				v.x = x - b.x;
				v.y = y - b.y;
				v.z = z - b.z;
				return v;
			}

			Vector3& multiply(const Vector3& v){
				x *= v.x;
				y *= v.y;
				z *= v.z;
				return *this;
			}

			inline Vector3& operator*=(const Vector3& v){
				return multiply(v);
			}

			Vector3& mutliply(T scalar){
				x *= scalar;
				y *= scalar;
				z *= scalar;
				return *this;
			}

			inline Vector3& operator*=(T scalar){
				return multiply(scalar);
			}

			Vector3& multiplyVectors(const Vector3& a, const Vector3& b){
				x = a.x * b.x;
				y = a.y * b.y;
				z = a.x * b.z;

				return *this;
			}

			Vector3 operator*(T s){
				Vector3 v;
				v.x = x * s;
				v.y = y * s;
				v.z = z * s;
				return v;
			}

			Vector3 operator*(const Vector3& b){
				Vector3 v;
				v.x = x * b.x;
				v.y = y * b.y;
				v.z = z * b.z;
				return v;
			}

			Vector3& applyEuler(const Euler&);
			Vector3& applyAxisAngle(const Vector3& axis, const T angle);
			Vector3& applyMatrix3(const Matrix3&);
			Vector3& applyMatrix4(const Matrix4&);
			Vector3& applyProjection(const Matrix4&);
			Vector3& applyQuaternion(const Quaternion&);
			Vector3& project(const Camera&);
			Vector3& unproject(const Camera&);
			Vector3& transformDirection(const Matrix4&);

			Vector3& divide(const Vector3& v){
				x /= v.x;
				y /= v.y;
				z /= v.z;
				return *this;
			}

			inline Vector3& operator/=(const Vector3& v){
				return divide(v);
			}

			Vector3& divide(T scalar){
				x /= scalar;
				y /= scalar;
				z /= scalar;
				return *this;
			}

			inline Vector3& operator/=(T scalar){
				return divide(scalar);
			}

			Vector3 operator/(const Vector3& v){
				Vector3 b;
				b.x = x / v.x;
				b.y = y / v.y;
				b.z = z / v.z;
				return b;
			}

			Vector3 operator/(T s){
				Vector3 v;
				v.x = x / s;
				v.y = y / s;
				v.z = z / s;
				return v;
			}

			Vector3& min(const Vector3&);
			Vector3& max(const Vector3&);
			Vector3& clamp(const Vector3& min, const Vector3& max);
			Vector3& clampScalar(T min, T max);
			Vector3& floor();
			Vector3& ceil();
			Vector3& round();
			Vector3& roundToZero();
			
			Vector3& negate(){
				x = -x;
				y = -y;
				z = -x;

				return *this;
			}

			inline Vector3 operator-(){
				negate();
			}
			
			T dot(const Vector3& v){
				return x * v.x + y * v.y + z * v.z;
			}

			T lengthSq(){
				return x * x + y * y + z * z;
			}

			T length(){
				return std::sqrt(lengthSq());
			}

			T lenghtManhattan(){
				return std::abs(x) + std::abs(y) + std::abs(z);
			}

			T normalize(){
				divide(length());
			}

			Vector3& setLength(T l){
				T oldLength = length();

				if (oldLength != 0 && l != oldLength){
					multiply(l / oldLength);
				}

				return this;
			}

			Vector3& lerp(const Vector3& v, T alpha){
				x += (v.x - x) * alpha;
				y += (v.y - y) * alpha;
				z += (v.z - z) * alpha;

				return *this;
			}

			Vector3& lerpVectors(const Vector3& v1, const Vector3& v2, T alpha){
				subVectors(v2, v1).multiply(alpha).add(v1);

				return *this;
			}

			Vector3& cross(const Vector3& v){
				T x = this->x, y = this->y, z = this->z;
				this->x = y * v.z - z * v.y;
				this->y = z * v.x - x * v.z;
				this->z = x * v.y - y * v.x;

				return *this;
			}

			Vector3& cross(const Vector3& a, const Vector3& b){

				x = a.y * b.z - a.z * b.y;
				y = a.z * b.x - a.x * b.z;
				z = a.x * b.y - a.y * b.x;

				return *this;

			}

			Vector3& projectOnVector(const Vector3&);
			Vector3& projectOnPlane(const Vector3& normal);
			Vector3& reflect(const Vector3& normal);
			T angleTo(const Vector3&);
			
			T distanceTo(const Vector3& v){
				return std::sqrt(distanceToSquared(v));
			}

			T distanceToSquared(const Vector3& v){
				T dx = x - v.x;
				T dy = y - v.y;
				T dz = z - v.z;

				return dx * dx + dy * dy + dz * dz;
			}

			Vector3& setFromMatrixPosition(const Matrix4&);
			Vector3& setFromMatrixScale(const Matrix4&);
			Vector3& setFromMatrixColumn(unsigned index, const Matrix4&);
			
			bool operator==(const Vector3& v){
				return ((x == v.x) && (y == v.y) && (z == v.z));
			}

			//TODO

		};
		
		template<typename T>
		Vector3<T> operator+(const T scalar, Vector3<T> rhs){
			return rhs + scalar;
		}

		template<typename T>
		Vector3<T> operator*(const T scalar, Vector3<T> rhs){
			return rhs * scalar;
		}

		typedef Vector3<> Vector3f;
		typedef Vector3<double> Vector3d;

	}

}

#endif

