#ifndef MYUPLAY_MYENGINE_MATRIX3
#define MYUPLAY_MYENGINE_MATRIX3

#include <vector>
#include <array>

#include "Vector3.hpp"
#include "Matrix3.hpp"

namespace MyUPlay {

	namespace MyEngine {

		//If Vector3 is not parsed yet. Circular dependency.
		#ifndef VECTOR3_DEFINED
		template <typename T> class Vector3;
		#endif
		#ifndef MATRIX4_DEFINED
		template <typename T> class Matrix4;
		#endif

		template<typename T = float>
		class Matrix3 {

		public:
			std::array<T,9> elements = {
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			};

			Matrix3(){};
			Matrix3(const Matrix3& m) {

				for (unsigned i = 0; i < 9; ++i){

					elements[i] = m.elements[i];

				}

			}

			Matrix3& operator=(const Matrix3& m){
				for (unsigned i = 0; i < 9; ++i){
					elements[i] = m.elements[i];
				}
			}

			Matrix3& set(T n11, T n12, T n13, T n21, T n22, T n23, T n31, T n32, T n33){

				T* te = elements;

				te[ 0 ] = n11; te[ 3 ] = n12; te[ 6 ] = n13;
				te[ 1 ] = n21; te[ 4 ] = n22; te[ 7 ] = n23;
				te[ 2 ] = n31; te[ 5 ] = n32; te[ 8 ] = n33;

				return *this;

			}

			Matrix3& identity(){
				set(
					1, 0, 0,
					0, 1, 0,
					0, 0, 1
				   );
				
				return *this;
			}

			std::vector<Vector3<T> >& applyToVector3Array(std::vector<Vector3<T> >& array, unsigned offset = 0, unsigned length = 0) const {
				
				Vector3<T> v1;

				if ( length == 0 ) length = array.size();

				for (unsigned i = 0; i < length - offset; i += 3) {
					v1.fromArray(array, i);
					v1.applyMatrix3(*this);
					v1.toArray(array, i);
				}

				return array;

			}

			Matrix3& multiply(T s){
				for (unsigned i = 0; i < 9; ++i){
					elements[i] *= s;
				}
				return *this;
			}

			Matrix3 operator*(T s){
				return Matrix3(*this).multiply(s);
			}

			Matrix3& operator*=(T s){
				return multiply(s);
			}

			T det() const {

				T& a = elements[ 0 ], b = elements[ 1 ], c = elements[ 2 ],
				   d = elements[ 3 ], e = elements[ 4 ], f = elements[ 5 ],
				   g = elements[ 6 ], h = elements[ 7 ], i = elements[ 8 ]; 

				return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;

			}

			Matrix3& getInverse(const Matrix4<T>&, bool throwOnInvertible);
			
			Matrix3& transpose(){
				T tmp;
				T* m = elements;

				tmp = m[ 1 ]; m[ 1 ] = m[ 3 ]; m[ 3 ] = tmp;
				tmp = m[ 2 ]; m[ 2 ] = m[ 6 ]; m[ 6 ] = tmp;
				tmp = m[ 5 ]; m[ 5 ] = m[ 7 ]; m[ 7 ] = tmp;

				return *this;
			}

			std::vector<T> flattenToArrayOffset(std::vector<T>& array, unsigned offset) const {

				array.reserve(offset + 8);

				for (unsigned i = 0; i < 9; ++i){
					array[offset + i] = elements[i];
				}

			}

			Matrix3& getNormalMatrix(const Matrix4<T>& m){

				return getInverse(m).transpose();

			}

			Matrix3& transposeIntoArray(std::vector<T>& r) const {

				T* m = elements;

				r[ 0 ] = m[ 0 ];
				r[ 1 ] = m[ 3 ];
				r[ 2 ] = m[ 6 ];
				r[ 3 ] = m[ 1 ];
				r[ 4 ] = m[ 4 ];
				r[ 5 ] = m[ 7 ];
				r[ 6 ] = m[ 2 ];
				r[ 7 ] = m[ 5 ];
				r[ 8 ] = m[ 8 ];

				return *this;

			}

			Matrix3& fromArray(const std::array<T, 9>& array){
				elements = array;
			}

			std::array<T, 9> toArray() const {

				return elements; //This will be copied.

			}


		};

		#define MATRIX3_DEFINED

		typedef Matrix3<float> Matrix3f;
		typedef Matrix3<double> Matrix3d;

	}

}

#endif

