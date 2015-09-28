
#include "Matrix3.hpp"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace MyUPlay::MyEngine;
using namespace std;

template <typename T>
Matrix3<T>& Matrix3<T>::getInverse(const Matrix4<T>& m, bool throwOnInvertable){

	T* me = m.elements;
	T te[9] = elements;

	te[ 0 ] =   me[ 10 ] * me[ 5 ] - me[ 6 ] * me[ 9 ];
	te[ 1 ] = - me[ 10 ] * me[ 1 ] + me[ 2 ] * me[ 9 ];
	te[ 2 ] =   me[ 6 ] * me[ 1 ] - me[ 2 ] * me[ 5 ];
	te[ 3 ] = - me[ 10 ] * me[ 4 ] + me[ 6 ] * me[ 8 ];
	te[ 4 ] =   me[ 10 ] * me[ 0 ] - me[ 2 ] * me[ 8 ];
	te[ 5 ] = - me[ 6 ] * me[ 0 ] + me[ 2 ] * me[ 4 ];
	te[ 6 ] =   me[ 9 ] * me[ 4 ] - me[ 5 ] * me[ 8 ];
	te[ 7 ] = - me[ 9 ] * me[ 0 ] + me[ 1 ] * me[ 8 ];
	te[ 8 ] =   me[ 5 ] * me[ 0 ] - me[ 1 ] * me[ 4 ];

	T det = me[ 0 ] * te[ 0 ] + me[ 1 ] * te[ 3 ] + me[ 2 ] * te[ 6 ];

	//no inverse
	
	if (det == 0){

		string msg = "Matrix3.getInverse(): can't invert matrix, determinant is 0";

		if (throwOnInvertable) {
			throw logic_error(msg);
		} else {
			cerr << msg << endl;
		}

		identity();

		return *this;

	}

	multiply( 1.0 / det );

	return *this;

}

