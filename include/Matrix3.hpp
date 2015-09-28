#ifndef MYUPLAY_MYENGINE_MATRIX3
#define MYUPLAY_MYENGINE_MATRIX3

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
			Matrix3(const Matrix3& m);

		};

		typedef Matrix3<float> Matrix3f;
		typedef Matrix3<double> Matrix3d;

	}

}

#endif

