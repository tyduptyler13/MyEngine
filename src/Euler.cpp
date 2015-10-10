#include <stdexcept>
#include <cmath>

#include "Euler.hpp"
#include "Math.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

template <typename T>
Euler<T>& Euler<T>::setFromRotationMatrix(const Matrix4<T>& m, Order o, bool update) {

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
		throw logic_error("Illegal state!");
	}

	order = o;

	if (update) onChangeCallback();

	return *this;

}

