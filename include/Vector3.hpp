#ifndef MYUPLAY_MYENGINE_VECTOR3
#define MYUPLAY_MYENGINE_VECTOR3


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
			//TODO


		};

	}

}

#endif

