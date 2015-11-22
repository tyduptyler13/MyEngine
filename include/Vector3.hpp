#ifndef MYUPLAY_MYENGINE_VECTOR3
#define MYUPLAY_MYENGINE_VECTOR3

#include <vector>
#include <cmath>

#include "Matrix3.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "Euler.hpp"
#include "Camera.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef MATRIX3_DEFINED
		template <typename> class Matrix3;
		#endif
		#ifndef MATRIX4_DEFINED
		template <typename> class Matrix4;
		#endif
		#ifndef CAMERA_DEFINED
		template <typename> class Camera;
		#endif

		/**
		 * This class supports any numeric type or class that supports
		 * all of the standard opperators for floats or doubles.
		 */
		template <typename T = float>
		class Vector3 {

		public:

			T x, y, z;

			Vector3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

			template <typename T2> //Allows conversion from other template types.
			Vector3(const Vector3<T2>& v){
				x = v.x;
				y = v.y;
				z = v.z;
			}

			Vector3& set(T x, T y, T z){
				this->x = x;
				this->y = y;
				this->z = z;
				return *this;
			}

			template <typename T2>
			inline Vector3& operator=(const Vector3<T2>& v){
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

			template<typename T2> //Allows conversion from other template types.
			Vector3& copy(const Vector3<T2>& v){
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

			Vector3 operator+(const Vector3& b) const {
				Vector3 v;
				v.x = x + b.x;
				v.y = y + b.y;
				v.z = z + b.z;
				return v;
			}

			inline Vector3 operator+(T s) const {
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

			Vector3 operator-(T s) const {
				Vector3 v;
				v.x = x * s;
				v.y = y * s;
				v.z = z * s;
				return v;
			}

			Vector3 operator-(const Vector3& b) const {
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

			Vector3 operator*(T s) const {
				Vector3 v;
				v.x = x * s;
				v.y = y * s;
				v.z = z * s;
				return v;
			}

			Vector3 operator*(const Vector3& b) const {
				Vector3 v;
				v.x = x * b.x;
				v.y = y * b.y;
				v.z = z * b.z;
				return v;
			}

			Vector3& applyEuler(const Euler<T>& e){
				  
				Quaternion<T> q;

				applyQuaternion(q.setFromEuler(e));

				return *this;

			}

			Vector3& applyAxisAngle(const Vector3& axis, const T angle){

				Quaternion<T> q;

				applyQuaternion(q.setFromAxisAngle(axis, angle));

				return *this;

			}

			Vector3& applyMatrix3(const Matrix3<T>&);
			Vector3& applyMatrix4(const Matrix4<T>&);
			Vector3& applyProjection(const Matrix4<T>&);
			Vector3& applyQuaternion(const Quaternion<T>&);
			Vector3& project(const Camera<T>&);
			Vector3& unproject(const Camera<T>&);
			Vector3& transformDirection(const Matrix4<T>&);

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

			Vector3 operator/(const Vector3& v) const {
				Vector3 b;
				b.x = x / v.x;
				b.y = y / v.y;
				b.z = z / v.z;
				return b;
			}

			Vector3 operator/(T s) const {
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
			
			Vector3& floor(){

				x = std::floor(x);
				y = std::floor(y);
				z = std::floor(z);

				return *this;

			}

			Vector3& ceil(){

				x = std::ceil(x);
				y = std::ceil(y);
				z = std::ceil(z);

				return *this;

			}

			Vector3& round(){

				x = std::round(x);
				y = std::round(y);
				z = std::round(z);

				return *this;

			}

			Vector3& roundToZero() {

				x = x < 0 ? std::ceil(x) : std::floor(x);
				y = y < 0 ? std::ceil(y) : std::floor(y);
				z = z < 0 ? std::ceil(z) : std::floor(z);

				return *this;

			}
			
			Vector3& negate(){
				x = -x;
				y = -y;
				z = -x;

				return *this;
			}

			inline Vector3 operator-() const {
				return Vector3(*this).negate();
			}
			
			T dot(const Vector3& v) const {
				return x * v.x + y * v.y + z * v.z;
			}

			T lengthSq() const {
				return x * x + y * y + z * z;
			}

			T length() const {
				return std::sqrt(lengthSq());
			}

			T lenghtManhattan() const {
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
			T angleTo(const Vector3&) const ;
			
			T distanceTo(const Vector3& v) const {
				return std::sqrt(distanceToSquared(v));
			}

			T distanceToSquared(const Vector3& v) const {
				T dx = x - v.x;
				T dy = y - v.y;
				T dz = z - v.z;

				return dx * dx + dy * dy + dz * dz;
			}

			Vector3& setFromMatrixPosition(const Matrix4<T>& m){
				x = m.elements[12];
				y = m.elements[13];
				z = m.elements[14];

				return this;
			}

			Vector3& setFromMatrixScale(const Matrix4<T>& m){
				
				T e[] = m.elements;

				T sx = set(e[0], e[1], e[2]).length();
				T sy = set(e[4], e[5], e[6]).length();
				T sz = set(e[8], e[9], e[10]).length();

				x = sx;
				y = sy;
				z = sz;

				return *this;
			}

			Vector3& setFromMatrixColumn(unsigned index, const Matrix4<T>& m) {

				unsigned offset = index * 4;

				x = m.elements[offset];
				y = m.elements[offset + 1];
				z = m.elements[offset + 2];

				return *this;

			}
			
			bool operator==(const Vector3& v) const {
				return ((x == v.x) && (y == v.y) && (z == v.z));
			}

			//Disabled until we decide the best method for moving data into an array/vector
			//std::vector<T>& toArray(T array[], unsigned offset);
			//Vector3& fromArray(T array[], unsigned offset);

		};

		#define VECTOR3_DEFINED

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

