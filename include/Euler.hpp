#ifndef MYUPLAY_MYENGINE_EULER
#define MYUPLAY_MYENGINE_EULER

#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "Vector3.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Euler {

		public:

			enum Order {
				XYZ,
				YZX,
				ZXY,
				XZY,
				YXZ,
				ZYX
			};

			T x, y, z;
			Order order;

			Euler(T x = 0, T y = 0, T z = 0, Order order = Order::XYZ)
			: x(x), y(y), z(z), order(order){}

			Euler(const Euler& e){
				copy(e);
			}

			Euler& copy(const Euler& e){
				x = e.x;
				y = e.y;
				z = e.z;
				order = e.order;
				return *this;
			}

			Euler& operator=(const Euler& e){
				return copy(e);
			}

			Euler& setFromRotationMatrix(const Matrix4<T>&, Order, bool update = false);



		};

	}

}

#endif

