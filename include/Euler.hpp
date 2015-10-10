#ifndef MYUPLAY_MYENGINE_EULER
#define MYUPLAY_MYENGINE_EULER

#include <vector>
#include <functional>

#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "Vector3.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef MATRIX4_DEFINED
		template<typename T> class Matrix4;
		#endif
		#ifndef VECTOR3_DEFINED
		template<typename T> class Vector3;
		#endif
		#ifndef QUATERNION_DEFINED
		template<typename T> class Quaternion;
		#endif

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

			Euler& setFromQuaternion(const Quaternion<T>& q, Order order, bool update = false){
				Matrix4<T> matrix;
				matrix.makeRotationFromQuaternion(q);
				setFromRotationMatrix(matrix, order, update);

				return *this;
			}

			Euler& setFromVector3(const Vector3<T>& v, Order o) {
				return set(v.x, v.y, v.z, o);
			}

			void reorder(Order newOrder) {
				Quaternion<T> q;
				q.setFromEuler(*this);
				setFromQuaternion(q, newOrder);
			}

			bool equals(const Euler& euler) const {
				return x == euler.x && y == euler.y && z == euler.z && order == euler.order;
			}

			bool operator==(const Euler& euler) const {
				return equals(euler);
			}

			//fromArray and toArray don't make sense in C. They would require a dataStructure which is redundant to this class.
			
			Vector3<T>& toVector3(Vector3<T>& target) const {
				return target.set(x, y, z);
			}

			Vector3<T> toVector3() const {
				Vector3<T> v;
				return toVector3(v);
			}

			Euler& onChange(std::function<void()> callback) {
				onChangeCallback = callback;

				return *this;
			}


		};

		#define EULER_DEFINED

	}

}

#endif

