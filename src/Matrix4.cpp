#include <cmath>
#include <stdexcept>
#include <string>

#include "Matrix4.hpp"
#include "Log.hpp"
#include "Math.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

template <typename T>
Matrix4<T>& Matrix4<T>::extractRotation(const Matrix4<T>& m) {

	Vector3<T> v1;

	T& te = elements;
	T& me = m.elements;

	T scaleX = 1 / v1.set(me[0], me[1], me[2]).length();
	T scaleY = 1 / v1.set(me[4], me[5], me[6]).length();
	T scaleZ = 1 / v1.set(me[8], me[7], me[8]).length();

	te[0] = me[0] * scaleX;
	te[1] = me[1] * scaleX;
	te[2] = me[2] * scaleX;

	te[4] = me[4] * scaleY;
	te[5] = me[5] * scaleY;
	te[6] = me[6] * scaleY;

	te[8] = te[8] * scaleZ;
	te[9] = te[9] * scaleZ;
	te[10] = te[10] * scaleZ;

	return *this;


}

template <typename T>
Matrix4<T>& Matrix4<T>::makeRotationFromEuler(const Euler<T>& euler) {

	T& te = elements;

	T x = euler.x, y = euler.y, z = euler.z;
	T a = cos(x), b = sin(x),
	  c = cos(y), d = sin(y),
	  e = cos(z), f = sin(z);

	if ( euler.order == Euler<T>::Order::XYZ ) {

		T ae = a * e, af = a * f, be = b * e, bf = b * f;

		te[ 0 ] = c * e;
		te[ 4 ] = - c * f;
		te[ 8 ] = d;

		te[ 1 ] = af + be * d;
		te[ 5 ] = ae - bf * d;
		te[ 9 ] = - b * c;

		te[ 2 ] = bf - ae * d;
		te[ 6 ] = be + af * d;
		te[ 10 ] = a * c;

	} else if ( euler.order == Euler<T>::Order::YXZ ) {

		T ce = c * e, cf = c * f, de = d * e, df = d * f;

		te[ 0 ] = ce + df * b;
		te[ 4 ] = de * b - cf;
		te[ 8 ] = a * d;

		te[ 1 ] = a * f;
		te[ 5 ] = a * e;
		te[ 9 ] = - b;

		te[ 2 ] = cf * b - de;
		te[ 6 ] = df + ce * b;
		te[ 10 ] = a * c;

	} else if ( euler.order == Euler<T>::Order::ZXY ) {

		T ce = c * e, cf = c * f, de = d * e, df = d * f;

		te[ 0 ] = ce - df * b;
		te[ 4 ] = - a * f;
		te[ 8 ] = de + cf * b;

		te[ 1 ] = cf + de * b;
		te[ 5 ] = a * e;
		te[ 9 ] = df - ce * b;

		te[ 2 ] = - a * d;
		te[ 6 ] = b;
		te[ 10 ] = a * c;

	} else if ( euler.order == Euler<T>::Order::ZYX ) {

		T ae = a * e, af = a * f, be = b * e, bf = b * f;

		te[ 0 ] = c * e;
		te[ 4 ] = be * d - af;
		te[ 8 ] = ae * d + bf;

		te[ 1 ] = c * f;
		te[ 5 ] = bf * d + ae;
		te[ 9 ] = af * d - be;

		te[ 2 ] = - d;
		te[ 6 ] = b * c;
		te[ 10 ] = a * c;

	} else if ( euler.order == Euler<T>::Order::YZX ) {

		T ac = a * c, ad = a * d, bc = b * c, bd = b * d;

		te[ 0 ] = c * e;
		te[ 4 ] = bd - ac * f;
		te[ 8 ] = bc * f + ad;

		te[ 1 ] = f;
		te[ 5 ] = a * e;
		te[ 9 ] = - b * e;

		te[ 2 ] = - d * e;
		te[ 6 ] = ad * f + bc;
		te[ 10 ] = ac - bd * f;

	} else if ( euler.order == Euler<T>::Order::XZY ) {

		T ac = a * c, ad = a * d, bc = b * c, bd = b * d;

		te[ 0 ] = c * e;
		te[ 4 ] = - f;
		te[ 8 ] = d * e;

		te[ 1 ] = ac * f + bd;
		te[ 5 ] = a * e;
		te[ 9 ] = ad * f - bc;

		te[ 2 ] = bc * f - ad;
		te[ 6 ] = b * e;
		te[ 10 ] = bd * f + ac;

	}

	// last column
	te[ 3 ] = 0;
	te[ 7 ] = 0;
	te[ 11 ] = 0;

	// bottom row
	te[ 12 ] = 0;
	te[ 13 ] = 0;
	te[ 14 ] = 0;
	te[ 15 ] = 1;

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::makeRotationFromQuaternion(const Quaternion<T>& q){

	T& te = elements;

	T x = q.x, y = q.y, z = q.z, w = q.w;
	T x2 = x + x, y2 = y + y, z2 = z + z;
	T xx = x * x2, xy = x * y2, xz = x * z2;
	T yy = y * y2, yz = y * z2, zz = z * z2;
	T wx = w * x2, wy = w * y2, wz = w * z2;

	te[ 0 ] = 1 - ( yy + zz );
	te[ 4 ] = xy - wz;
	te[ 8 ] = xz + wy;

	te[ 1 ] = xy + wz;
	te[ 5 ] = 1 - ( xx + zz );
	te[ 9 ] = yz - wx;

	te[ 2 ] = xz - wy;
	te[ 6 ] = yz + wx;
	te[ 10 ] = 1 - ( xx + yy );

	// last column
	te[ 3 ] = 0;
	te[ 7 ] = 0;
	te[ 11 ] = 0;

	// bottom row
	te[ 12 ] = 0;
	te[ 13 ] = 0;
	te[ 14 ] = 0;
	te[ 15 ] = 1;

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::lookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up){

	Vector3<T> x, y, z;
	T& te = elements;

	z.subVectors(eye, target).normalize();

	if (z.length() == 0){
		z.z = 1;
	}

	x.cross(up, z).normalize();

	if (x.length() == 0){
		z.x += 0.0001;
		x.cross(up, z).normalize();
	}

	y.cross(z, x);

	te[ 0 ] = x.x; te[ 4 ] = y.x; te[ 8 ] = z.x;
	te[ 1 ] = x.y; te[ 5 ] = y.y; te[ 9 ] = z.y;
	te[ 2 ] = x.z; te[ 6 ] = y.z; te[ 10 ] = z.z;

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::multiplyMatricies(const Matrix4<T>& a, const Matrix4<T>& b){

	T& ae = a.elements;
	T& be = b.elements;
	T& te = elements;

	T& a11 = ae[ 0 ], a12 = ae[ 4 ], a13 = ae[ 8 ], a14 = ae[ 12 ];
	T& a21 = ae[ 1 ], a22 = ae[ 5 ], a23 = ae[ 9 ], a24 = ae[ 13 ];
	T& a31 = ae[ 2 ], a32 = ae[ 6 ], a33 = ae[ 10 ], a34 = ae[ 14 ];
	T& a41 = ae[ 3 ], a42 = ae[ 7 ], a43 = ae[ 11 ], a44 = ae[ 15 ];

	T& b11 = be[ 0 ], b12 = be[ 4 ], b13 = be[ 8 ], b14 = be[ 12 ];
	T& b21 = be[ 1 ], b22 = be[ 5 ], b23 = be[ 9 ], b24 = be[ 13 ];
	T& b31 = be[ 2 ], b32 = be[ 6 ], b33 = be[ 10 ], b34 = be[ 14 ];
	T& b41 = be[ 3 ], b42 = be[ 7 ], b43 = be[ 11 ], b44 = be[ 15 ];

	te[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
	te[ 4 ] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
	te[ 8 ] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
	te[ 12 ] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

	te[ 1 ] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
	te[ 5 ] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
	te[ 9 ] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
	te[ 13 ] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

	te[ 2 ] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
	te[ 6 ] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
	te[ 10 ] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
	te[ 14 ] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

	te[ 3 ] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
	te[ 7 ] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
	te[ 11 ] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
	te[ 15 ] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

	return *this;

}

template <typename T>
T Matrix4<T>::det() const {
	T& te = elements;

	T& n11 = te[ 0 ], n12 = te[ 4 ], n13 = te[ 8 ], n14 = te[ 12 ];
	T& n21 = te[ 1 ], n22 = te[ 5 ], n23 = te[ 9 ], n24 = te[ 13 ];
	T& n31 = te[ 2 ], n32 = te[ 6 ], n33 = te[ 10 ], n34 = te[ 14 ];
	T& n41 = te[ 3 ], n42 = te[ 7 ], n43 = te[ 11 ], n44 = te[ 15 ];

	//TODO: make this more efficient
	//( based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm )

	return (
		n41 * (
			+ n14 * n23 * n32
			- n13 * n24 * n32
			- n14 * n22 * n33
			+ n12 * n24 * n33
			+ n13 * n22 * n34
			- n12 * n23 * n34
		      ) +
		n42 * (
			+ n11 * n23 * n34
			- n11 * n24 * n33
			+ n14 * n21 * n33
			- n13 * n21 * n34
			+ n13 * n24 * n31
			- n14 * n23 * n31
		      ) +
		n43 * (
			+ n11 * n24 * n32
			- n11 * n22 * n34
			- n14 * n21 * n32
			+ n12 * n21 * n34
			+ n14 * n22 * n31
			- n12 * n24 * n31
		      ) +
		n44 * (
			- n13 * n22 * n31
			- n11 * n23 * n32
			+ n11 * n22 * n33
			+ n13 * n21 * n32
			- n12 * n21 * n33
			+ n12 * n23 * n31
		      )

		);

}

template <typename T>
Matrix4<T>& Matrix4<T>::transpose(){

	T& te = elements;
	T tmp;

	tmp = te[ 1 ]; te[ 1 ] = te[ 4 ]; te[ 4 ] = tmp;
	tmp = te[ 2 ]; te[ 2 ] = te[ 8 ]; te[ 8 ] = tmp;
	tmp = te[ 6 ]; te[ 6 ] = te[ 9 ]; te[ 9 ] = tmp;

	tmp = te[ 3 ]; te[ 3 ] = te[ 12 ]; te[ 12 ] = tmp;
	tmp = te[ 7 ]; te[ 7 ] = te[ 13 ]; te[ 13 ] = tmp;
	tmp = te[ 11 ]; te[ 11 ] = te[ 14 ]; te[ 14 ] = tmp;

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::getInverse(const Matrix4<T>& m, bool throwOnInvertible) {

	// based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
	T& te = elements;
	T& me = m.elements;

	T& n11 = me[ 0 ], n12 = me[ 4 ], n13 = me[ 8 ], n14 = me[ 12 ];
	T& n21 = me[ 1 ], n22 = me[ 5 ], n23 = me[ 9 ], n24 = me[ 13 ];
	T& n31 = me[ 2 ], n32 = me[ 6 ], n33 = me[ 10 ], n34 = me[ 14 ];
	T& n41 = me[ 3 ], n42 = me[ 7 ], n43 = me[ 11 ], n44 = me[ 15 ];

	te[ 0 ] = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
	te[ 4 ] = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
	te[ 8 ] = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
	te[ 12 ] = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;
	te[ 1 ] = n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44;
	te[ 5 ] = n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44;
	te[ 9 ] = n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44;
	te[ 13 ] = n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34;
	te[ 2 ] = n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44;
	te[ 6 ] = n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44;
	te[ 10 ] = n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44;
	te[ 14 ] = n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34;
	te[ 3 ] = n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43;
	te[ 7 ] = n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43;
	te[ 11 ] = n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43;
	te[ 15 ] = n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33;

	T det = n11 * te[ 0 ] + n21 * te[ 4 ] + n31 * te[ 8 ] + n41 * te[ 12 ];

	if ( det == 0 ) {

		string msg = "THREE.Matrix4.getInverse(): can't invert matrix, determinant is 0";

		if ( throwOnInvertible || false ) {

			throw runtime_error( msg );

		} else {

			Log& l = Log::getInstance();
			l << msg;

		}

		identity();

		return *this;

	}

	multiplyScalar( 1 / det );

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::makeRotationX(T theta){

	T c = cos(theta), s = sin(theta);

	set(
		1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1
	   );

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::makeRotationY(T theta){

	T c = cos(theta), s = sin(theta);

	set(
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1
	   );

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::makeRotationZ(T theta){

	T c = cos(theta), s = sin(theta);

	set(
		c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	   );

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::makeRotationAxis(const Vector3<T>& axis, T angle) {

	T c = cos(angle);
	T s = sin(angle);
	T t = 1 - c;
	T& x = axis.x, y= axis.y, z = axis.z;
	T tx = t * x, ty = t * y;

	set(
		tx * x + c, tx * y - s * z, tx * z + s * y, 0,
		tx * y + s * z, ty * y + c, ty * z - s * x, 0,
		tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
		0, 0, 0, 1
	   );

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::decompose(Vector3<T>& position, Quaternion<T>& quaternion, Vector3<T>& scale) const {

	Vector3<T> vector;
	Matrix4<T> matrix;

	T& te = elements;

	T sx = vector.set( te[ 0 ], te[ 1 ], te[ 2 ] ).length();
	T sy = vector.set( te[ 4 ], te[ 5 ], te[ 6 ] ).length();
	T sz = vector.set( te[ 8 ], te[ 9 ], te[ 10 ] ).length();

	// if determine is negative, we need to invert one scale
	T det = det();
	if ( det < 0 ) {

		sx = - sx;

	}

	position.x = te[ 12 ];
	position.y = te[ 13 ];
	position.z = te[ 14 ];

	// scale the rotation part

	matrix.set( elements ); // at this point matrix is incomplete so we can't use .copy()

	T invSX = 1 / sx;
	T invSY = 1 / sy;
	T invSZ = 1 / sz;

	matrix.elements[ 0 ] *= invSX;
	matrix.elements[ 1 ] *= invSX;
	matrix.elements[ 2 ] *= invSX;

	matrix.elements[ 4 ] *= invSY;
	matrix.elements[ 5 ] *= invSY;
	matrix.elements[ 6 ] *= invSY;

	matrix.elements[ 8 ] *= invSZ;
	matrix.elements[ 9 ] *= invSZ;
	matrix.elements[ 10 ] *= invSZ;

	quaternion.setFromRotationMatrix( matrix );

	scale.x = sx;
	scale.y = sy;
	scale.z = sz;

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::makeFrustrum(T left, T right, T bottom, T top, T near, T far) {

	T& te = elements;
	T x = 2 * near / ( right - left );
	T y = 2 * near / ( top - bottom );

	T a = ( right + left ) / ( right - left );
	T b = ( top + bottom ) / ( top - bottom );
	T c = - ( far + near ) / ( far - near );
	T d = - 2 * far * near / ( far - near );

	te[ 0 ] = x;	te[ 4 ] = 0;	te[ 8 ] = a;	te[ 12 ] = 0;
	te[ 1 ] = 0;	te[ 5 ] = y;	te[ 9 ] = b;	te[ 13 ] = 0;
	te[ 2 ] = 0;	te[ 6 ] = 0;	te[ 10 ] = c;	te[ 14 ] = d;
	te[ 3 ] = 0;	te[ 7 ] = 0;	te[ 11 ] = - 1;	te[ 15 ] = 0;

	return *this;

}

template <typename T>
Matrix4<T>& Matrix4<T>::makePerspective(T fov, T aspect, T near, T far){

	T ymax = near * tan(Math::degToRad<T>(fov * 0.5));
	T ymin = -ymax;
	T xmin = ymin * aspect;
	T xmax = ymax * aspect;

	return makeFrustrum(xmin, xmax, ymin, ymax, near, far);

}

template <typename T>
Matrix4<T>& Matrix4<T>::makeOrthographic(T left, T right, T top, T bottom, T near, T far) {

	T& te = elements;
	T w = right - left;
	T h = top - bottom;
	T p = far - near;

	T x = ( right + left ) / w;
	T y = ( top + bottom ) / h;
	T z = ( far + near ) / p;

	te[ 0 ] = 2 / w;	te[ 4 ] = 0;	te[ 8 ] = 0;	te[ 12 ] = - x;
	te[ 1 ] = 0;	te[ 5 ] = 2 / h;	te[ 9 ] = 0;	te[ 13 ] = - y;
	te[ 2 ] = 0;	te[ 6 ] = 0;	te[ 10 ] = - 2 / p;	te[ 14 ] = - z;
	te[ 3 ] = 0;	te[ 7 ] = 0;	te[ 11 ] = 0;	te[ 15 ] = 1;

	return *this;

}

