#ifndef MYUPLAY_MYENGINE_QUATERNION
#define MYUPLAY_MYENGINE_QUATERNION

#include <functional>
#include <vector>

#include "Euler.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Quaternion {

		private:
			T x, y, z, w;

			std::function<void()> onChangeCallback = [](){return 0;}; //Does nothing but can be overwritten.

		public:

			Quaternion(T x = 0, T y = 0, T z = 0, T w = 1)
				:x(x), y(y), z(z), w(w){
				onChangeCallback(); //The constructor can be called after creation as a set.
			}

			Quaternion(const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w){
				onChangeCallback(); //The constructor can be called after creation as a copy.
			}

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

			void setX(T n) {
				x = n;
				onChangeCallback();
			}

			void setY(T n) {
				y = n;
				onChangeCallback();
			}

			void setZ(T n) {
				z = n;
				onChangeCallback();
			}

			void setW(T n) {
				w = n;
				onChangeCallback();
			}

			Quaternion& onChange(std::function<void()> callback){
				onChangeCallback = callback;
				return *this;
			}

			Quaternion& copy(const Quaternion& q){
				x = q.x;
				y = q.y;
				z = q.z;
				w = q.w;

				onChangeCallback();

				return *this;
			}

			Quaternion& operator=(const Quaternion& q){
				return copy(q);
			}

			Quaternion clone() const {
				return Quaternion(*this);
			}

			Quaternion& setFromEuler(const Euler<T>&, bool update);
			Quaternion& setFromAxisAngle(const Vector3<T>& axis, T angle);
			Quaternion& setFromRotationMatrix(const Matrix4<T>&);
			Quaternion& setFromUnitVectors(const Vector3& from, const Vector3& to);

			Quaternion& inverse() {
				conjugate().normalize();

				return *this;
			}

			Quaternion& conjugate(){
				x *= -1;
				y *= -1;
				z *= -1;

				onChangeCallback();

				return *this;

			}

			T dot(Quaternion& q) const {
				return x * q.x + q * q.y + z * q.z + w * q.w;
			}

			T lengthSq() const {
				return x * x + y * y + z * z + w * w;
			}

			T length() const; //Avoiding including math.h in header.

			Quaternion& normalize();

			Quaternion& multiply(const Quaternion& q) {
				return multiplyQuaternions(*this, q);
			}

			Quaternion& multiplyQuaternions(const Quaternion&, const Quaternion&);

			Quaternion& operator*=(const Quaterion& q){
				return multiply(q);
			}

			Quaternion operator*(const Quaternion& q) const {
				return Quaternion(*this).multiply(q);
			}

			Quaternion& slerp(const Quaternion&, T interpolation);

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

				onChangeCallback();

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
				return target(a).slerp(b, t);
			}

		};

	}

}

#endif

