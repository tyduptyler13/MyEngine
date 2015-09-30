#ifndef MYUPLAY_MYENGINE_SPHERE
#define MYUPLAY_MYENGINE_SPHERE

#include "Vector3.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Sphere {

		public:

			Vector3<T> center;
			T radius;

		};

	}

}

#endif

