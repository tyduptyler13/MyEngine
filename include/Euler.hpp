#ifndef MYUPLAY_MYENGINE_EULER
#define MYUPLAY_MYENGINE_EULER

#include <vector>
#include <functional>

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

		private:
			T x, y, z;
			Order order;

			std::function<void()> onChangeCallback = [](){return 0;};

		public:

			Euler(T x = 0, T y = 0, T z = 0, Order order = Order::XYZ)
				: x(x), y(y), z(z), order(order){}

			Euler(const Euler& e) : x(e.x), y(e.y), z(e.z), order(e.order) {}

			T getX() const {
				return x;
			}

			T getY() const {
				return y;
			}

			T getZ() const {
				return z;
			}

			Order getOrder() const {
				return order;
			}

			void setX(T n){
				x = n;
				onChangeCallback();
			}

			void setY(T n){
				y = n;
				onChangeCallback();
			}

			void setZ(T n){
				z = n;
				onChangeCallback();
			}

			void setOrder(Order o){
				order = o;
				onChangeCallback();
			}

			Euler& copy(const Euler& e){
				x = e.x;
				y = e.y;
				z = e.z;
				order = e.order;

				onChangeCallback();

				return *this;
			}

			Euler& operator=(const Euler& e){
				return copy(e);
			}

			Euler& setFromRotationMatrix(const Matrix4<T>&, Order, bool update = false);

			Euler& onChange(std::function<void()> callback) {
				onChangeCallback = callback;

				return *this;
			}


		};

	}

}

#endif

