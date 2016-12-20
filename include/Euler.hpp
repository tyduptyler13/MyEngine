#ifndef MYUPLAY_MYENGINE_EULER
#define MYUPLAY_MYENGINE_EULER

#include <vector>
#include <stdexcept>

namespace MyUPlay {

	namespace MyEngine {

		enum Order {
			XYZ,
			YZX,
			ZXY,
			XZY,
			YXZ,
			ZYX
		};

		template <typename T>
		struct Euler;

		typedef Euler<float> Eulerf;
		typedef Euler<double> Eulerd;
	}
}

#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Math.hpp"

template <typename T = float>
struct MyUPlay::MyEngine::Euler {

	T x, y, z;
	Order order;

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
	}

	void setY(T n){
		y = n;
	}

	void setZ(T n){
		z = n;
	}

	void setOrder(Order o){
		order = o;
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

	Euler& setFromRotationMatrix(const Matrix4<T>& m, Order order){

		T* te = m.elements;
		const T& m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ],
				m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ],
				m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ];

		if (order == Order::XYZ) {

			y = asin(Math::clamp(m13, -1, 1) );

			if (abs(m13) < 0.99999 ) {
				x = atan2( - m23, m33 );
				z = atan2( - m12, m11 );
			} else {
				x = atan2( m32, m22 );
				z = 0;
			}


		} else if (order == Order::YXZ) {

			x = asin( - Math::clamp(m23, -1, 1));

			if (abs(m23) < 0.99999 ) {

				y = atan2( m13, m33 );
				z = atan2( m21, m22 );

			} else {

				y = atan2 ( -m31, m11 );
				z = 0;

			}

		} else if (order == Order::ZXY) {
			x = asin( Math::clamp(m32, -1, 1 ));

			if (abs(m32) < 0.99999 ) {

				y = atan2(-m31, m33);
				z = atan2(-m12, m22);

			} else {
				y = 0;
				z = atan2(m21, m11);
			}

		} else if (order == Order::ZYX) {

			y = asin(-Math::clamp(m31, -1, 1));

			if (abs(m31)<0.99999){
				x = atan2(m32, m33);
				z = atan2(m21, m11);
			} else {
				x = 0;
				z = atan2(-m12, m22);
			}

		} else if (order == Order::YZX) {

			z = asin(Math::clamp(m21, -1, 1));

			if (abs(m21) < 0.99999 ){
				x = atan2(-m23, m22);
				y = atan2(-m31, m11);
			} else {
				x = 0;
				y = atan2(m13, m33);
			}
		} else if (order == Order::XZY) {
			z = asin( -clamp(m12, -1, 1));
			if (abs(m12) < 0.99999){
				x = atan2(m32, m22);
				y = atan2(m13, m11);
			} else {
				x = atan2(-m23, m33);
				y = 0;
			}
		} else {
			throw new std::logic_error("Illegal state!");
		}

		this->order = order;


		return *this;

	}

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

};

#endif
