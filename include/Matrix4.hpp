#ifndef MYUPLAY_MYENGINE_MATRIX4
#define MYUPLAY_MYENGINE_MATRIX4

#include <vector>

#include "Vector3.hpp"
#include "Euler.hpp"
#include "Quaternion.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Matrix4 {

			//Circular Dependencies
			#ifndef VECTOR3_DEFINED
			template <typename> class Vector3;
			#endif
			#ifndef MATRIX3_DEFINED
			template <typename> class Matrix3;
			#endif

		public:

			T elements[16] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};

			Matrix4(){}

			Matrix4(const Matrix4& m){
				for (unsigned i = 0; i < 16; ++i){
					elements[i] = m.elements[i];
				}
			}

			Matrix4& operator=(const Matrix4& m){
				for (unsigned i = 0; i < 16; ++i){
					elements[i] = m.elements[i];
				}
			}

			Matrix4& set(T n11, T n12, T n13, T n14, T n21, T n22, T n23, T n24, T n31, T n32, T n33, T n34, T n41, T n42, T n43, T n44) {
				T& te = elements;

				te[ 0 ] = n11; te[ 4 ] = n12; te[ 8 ] = n13; te[ 12 ] = n14;
				te[ 1 ] = n21; te[ 5 ] = n22; te[ 9 ] = n23; te[ 13 ] = n24;
				te[ 2 ] = n31; te[ 6 ] = n32; te[ 10 ] = n33; te[ 14 ] = n34;
				te[ 3 ] = n41; te[ 7 ] = n42; te[ 11 ] = n43; te[ 15 ] = n44;

				return *this;

			}

			Matrix4& identity(){

				set(
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
				   );

				return *this;

			}

			Matrix4& copyPosition(const Matrix4& m){

				elements[12] = m.elements[12];
				elements[13] = m.elements[13];
				elements[14] = m.elements[14];

				return *this;

			}

			Matrix4& extractBasis(Vector3<T>& xAxis, Vector3<T>& yAxis, Vector3<T>& zAxis) const {

				T& e = elements;

				xAxis.set(e[0], e[1], e[2]);
				yAxis.set(e[4], e[5], e[6]);
				zAxis.set(e[8], e[9], e[10]);

				return *this;

			}

			Matrix4& makeBasis(Vector3<T>& xAxis, Vector3<T>& yAxis, Vector3<T>& zAxis) {
				set(
					xAxis.x, yAxis.x, zAxis.x, 0,
					xAxis.y, yAxis.y, zAxis.y, 0,
					xAxis.z, yAxis.z, zAxis.z, 0,
					0,       0,       0,       1
				   );

				return *this;

			}

			Matrix4& extractRotation(const Matrix4&);

			Matrix4& makeRotationFromEuler(const Euler<T>&);

			Matrix4& makeRotationFromQuaternion(const Quaternion<T>&);

			Matrix4& lookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up);

			Matrix4& multiply(const Matrix4& m){
				multiplyMatricies(*this, m);
			}

			Matrix4& operator*=(const Matrix4& m){
				multiply(m);
			}

			Matrix4 operator*(const Matrix4& m) const {

				return Matrix4().multiplyMatricies(*this, m);

			}

			Matrix4& multiplyMatricies(const Matrix4& a, const Matrix4& b);

			std::vector<T>& multiplyToArray(const Matrix4& a, const Matrix4& b, std::vector<T>& array);
			std::vector<T> multiplyToArray(const Matrix4& a, const Matrix4& b){
				std::vector<T> array(16);
				return multiplyToArray(a, b, array);
			}


			Matrix4& multiplyScalar(T s) {

				T& te = elements;

				te[ 0 ] *= s; te[ 4 ] *= s; te[ 8 ] *= s; te[ 12 ] *= s;
				te[ 1 ] *= s; te[ 5 ] *= s; te[ 9 ] *= s; te[ 13 ] *= s;
				te[ 2 ] *= s; te[ 6 ] *= s; te[ 10 ] *= s; te[ 14 ] *= s;
				te[ 3 ] *= s; te[ 7 ] *= s; te[ 11 ] *= s; te[ 15 ] *= s;

				return *this;

			}

			std::vector<Vector3<T> > applyToVector3Array(std::vector<Vector3<T> >& array, unsigned offset = 0, unsigned length = 0) {

				Vector3<T> v1;

				if (length == 0) length = array.size();

				for(unsigned i = 0; i < length; i += 3){
					v1.fromArray(array, i + offset);
					v1.applyMatrix4(*this);
					v1.toArray(array, i + offset);
				}

				return *this;

			}

			T det() const;

			Matrix4& transpose();

			std::vector<T> flattenToArrayOffset(std::vector<T>& array, unsigned offset) {
				for(unsigned i = 0; i < 16; ++i){
					array[i = offset] = elements[i];
				}
			}

			Matrix4& setPosition(const Vector3<T>& v){
				elements[12] = v.x;
				elements[13] = v.y;
				elements[14] = v.z;

				return *this;
			}

			Matrix4& getInverse(const Matrix4&, bool throwOnInvertible = false);

			Matrix4& scale(const Vector3<T>& v){
				T& te = elements;
				T x = v.x, y = v.y, z = v.z;

				te[ 0 ] *= x; te[ 4 ] *= y; te[ 8 ] *= z;
				te[ 1 ] *= x; te[ 5 ] *= y; te[ 9 ] *= z;
				te[ 2 ] *= x; te[ 6 ] *= y; te[ 10 ] *= z;
				te[ 3 ] *= x; te[ 7 ] *= y; te[ 11 ] *= z;

				return *this;

			}

			Matrix4& makeTranslation(T x, T y, T z){

				set(
					1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1
				   );

				return *this;

			}

			Matrix4& makeRotationX(T theta);
			Matrix4& makeRotationY(T theta);
			Matrix4& makeRotationZ(T theta);
			Matrix4& makeRotationAxis(const Vector3<T>& axis, T angle);

			Matrix4& makeScale(T x, T y, T z) {

				set(
					x, 0, 0, 0,
					0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, 1
				   );

				return *this;

			}

			Matrix4& compose(const Vector3<T>& position, const Quaternion<T>& quaternion, const Vector3<T>& scale) {
				makeRotationFromQuaternion(quaternion);
				scale(scale);
				setPosition(position);
			}

			Matrix4& decompose(Vector3<T>& position, Quaternion<T>& quaternion, Vector3<T>& scale) const;

			Matrix4& makeFrustrum(T left, T right, T bottom, T top, T near, T far);
			Matrix4& makePerspective(T fov, T aspect, T near, T far);
			Matrix4& makeOrthographic(T left, T right, T top, T bottom, T near, T far);

			bool equals(const Matrix4& m) const {

				for (unsigned i = 0; i < 16; ++i){

					if (elements[i] == m.elements[i]) return false;
				}

				return true;

			}

			bool operator==(const Matrix4& m) const {
				return equals(m);
			}

			Matrix4& fromArray(const std::vector<T>& array){

				for (unsigned i = 0; i < 16; ++i){

					elements[i] = array[i];

				}

			}

			std::vector<T> toArray() const {

				std::vector<T> ret(16);

				for (unsigned i = 0; i < 16; ++i){
					ret[i] = elements[i];
				}

				return ret;

			}

		};

		#define MATRIX4_DEFINED

		typedef Matrix4<float> Matrix4f;
		typedef Matrix4<double> Matrix4d;

	}

}

#endif

