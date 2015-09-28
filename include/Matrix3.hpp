#ifndef MYUPLAY_MYENGINE_MATRIX3
#define MYUPLAY_MYENGINE_MATRIX3

#include <vector>

#include "Vector3.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template<typename T = float>
		class Matrix3 {

		public:
			T elements[9] = {
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

			Matrix3& set(T n11, T n12, T n13, T n21, T n22, T n23, T n31, T n32, T n33);

			Matrix3& identity(){
				set(
					1, 0, 0,
					0, 1, 0,
					0, 0, 1
				   );
				
				return *this;
			}

			std::vector<Vector3<T>> applyToVector3Array(std::vector<Vector3<T>> array, unsigned offset = 0, unsigned length = 0){
				
				Vector3<T> v1;

				if ( length == 0 ) length = array.size();

				for (unsigned i = 0; i < length - offset; i += 3) {
					v1.fromArray(array, i);
					v1.applyMatrix3(*this);
					v1.toArray(array, i);
				}

				return array;

			}



		};

		typedef Matrix3<float> Matrix3f;
		typedef Matrix3<double> Matrix3d;

	}

}

#endif

