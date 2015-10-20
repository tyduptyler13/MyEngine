#ifndef MYUPLAY_MYENGINE_FRUSTUM
#define MYUPLAY_MYENGINE_FRUSTUM

#include "Plane.hpp"
#include "Matrix4.hpp"

namespace MyUPlay {

	namespace MyEngine {

		//float based. No need for doubles, accuracy isn't that important here.
		class Frustum {

		public:

			Planef planes[6]; 

			Frustum(){

				for (Planef& plane : planes) {
					plane = Planef();
				}

			};

			//Copy constructor
			Frustum(const Frustum& f) {
				for (unsigned i = 0; i < 6; ++i){
					planes[i] = f.planes[i];
				}
			}

			Frustum& operator=(const Frustum& f) {

				for (unsigned i = 0; i < 6; ++i){
					planes[i] = f.planes[i];
				}

			}

			Frustum& setFromMatrix(const Matrix4f& matrix) {

				std::array<float, 16>& me =  matrix.elements me;
				var me0 = me[ 0 ], me1 = me[ 1 ], me2 = me[ 2 ], me3 = me[ 3 ];
				var me4 = me[ 4 ], me5 = me[ 5 ], me6 = me[ 6 ], me7 = me[ 7 ];
				var me8 = me[ 8 ], me9 = me[ 9 ], me10 = me[ 10 ], me11 = me[ 11 ];
				var me12 = me[ 12 ], me13 = me[ 13 ], me14 = me[ 14 ], me15 = me[ 15 ];
			}

		};

	}

}

#endif

