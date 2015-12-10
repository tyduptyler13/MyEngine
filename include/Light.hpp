#include "Object3D.hpp"
#include "Color.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Light : public Object3D<T> {

		public:

			Color color;

		};

	}

}

