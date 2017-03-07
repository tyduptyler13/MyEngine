#ifndef MYUPLAY_MYENGINE_MATRIX4
#define MYUPLAY_MYENGINE_MATRIX4

#include <vector>
#include <array>
#include <stdexcept>
#include <cmath>

namespace MyUPlay {
	namespace MyEngine {
		template <typename T> class Matrix4;
		typedef Matrix4<float> Matrix4f;
		typedef Matrix4<double> Matrix4d;
	}
}

#include "Vector3.hpp"
#include "Euler.hpp"
#include "Quaternion.hpp"
#include "Math.hpp"
#include "Log.hpp"

template <typename T = float>
class MyUPlay::MyEngine::Matrix4 {

private:

	Log console = Log("Matrix4");

public:

	std::array<T, 16> elements = { {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
	} };

	Matrix4(){}

	Matrix4(const Matrix4& m){
		elements = m.elements;
	}

	Matrix4& set(const Matrix4& m){
		elements = m.elements;
		return *this;
	}

	Matrix4& operator=(const Matrix4& m){
		elements = m.elements;
		return *this;
	}

	Matrix4& copy(const Matrix4& m){
		elements = m.elements;
		return *this;
	}

	Matrix4& set(T n11, T n12, T n13, T n14, T n21, T n22, T n23, T n24, T n31, T n32, T n33, T n34, T n41, T n42, T n43, T n44) {
		auto& te = elements;

		te[ 0 ] = n11; te[ 4 ] = n12; te[ 8 ] = n13; te[ 12 ] = n14;
		te[ 1 ] = n21; te[ 5 ] = n22; te[ 9 ] = n23; te[ 13 ] = n24;
		te[ 2 ] = n31; te[ 6 ] = n32; te[ 10 ] = n33; te[ 14 ] = n34;
		te[ 3 ] = n41; te[ 7 ] = n42; te[ 11 ] = n43; te[ 15 ] = n44;

		return *this;

	}

	Matrix4& identity(){

		set(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
		);

		return *this;

	}

	Matrix4& copyPosition(const Matrix4& m){

		elements[12] = m.elements[12];
		elements[13] = m.elements[13];
		elements[14] = m.elements[14];

		return *this;

	}

	const Matrix4& extractBasis(Vector3<T>& xAxis, Vector3<T>& yAxis, Vector3<T>& zAxis) const {

		auto& e = elements;

		xAxis.set(e[0], e[1], e[2]);
		yAxis.set(e[4], e[5], e[6]);
		zAxis.set(e[8], e[9], e[10]);

		return *this;

	}

	Matrix4& makeBasis(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis) {
		set(
				xAxis.x, yAxis.x, zAxis.x, 0,
				xAxis.y, yAxis.y, zAxis.y, 0,
				xAxis.z, yAxis.z, zAxis.z, 0,
				0,       0,       0,       1
		);

		return *this;

	}

	Matrix4& extractRotation(const Matrix4& m){

		Vector3<T> v1;

		auto& te = elements;
		auto& me = m.elements;

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

	Matrix4& makeRotationFromEuler(const Euler<T>& euler) {

		auto& te = elements;

		T x = euler.x, y = euler.y, z = euler.z;
		T a = cos(x), b = sin(x),
				c = cos(y), d = sin(y),
				e = cos(z), f = sin(z);

		if ( euler.order == Order::XYZ ) {

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

		} else if ( euler.order == Order::YXZ ) {

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

		} else if ( euler.order == Order::ZXY ) {

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

		} else if ( euler.order == Order::ZYX ) {

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

		} else if ( euler.order == Order::YZX ) {

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

		} else if ( euler.order == Order::XZY ) {

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

	Matrix4& makeRotationFromQuaternion(const Quaternion<T>& q) {

		auto& te = elements;

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

	Matrix4& lookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up){

		Vector3<T> x, y, z;
		auto& te = elements;

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

	Matrix4& multiply(const Matrix4& m){
		return multiplyMatrices(*this, m);
	}

	Matrix4& operator*=(const Matrix4& m){
		return multiply(m);
	}

	Matrix4 operator*(const Matrix4& m) const {
		return Matrix4().multiplyMatrices(*this, m);
	}

	Matrix4& multiplyMatrices(const Matrix4& a, const Matrix4& b){

		auto& ae = a.elements;
		auto& be = b.elements;
		auto& te = elements;

		const T& a11 = ae[ 0 ], a12 = ae[ 4 ], a13 = ae[ 8 ], a14 = ae[ 12 ];
		const T& a21 = ae[ 1 ], a22 = ae[ 5 ], a23 = ae[ 9 ], a24 = ae[ 13 ];
		const T& a31 = ae[ 2 ], a32 = ae[ 6 ], a33 = ae[ 10 ], a34 = ae[ 14 ];
		const T& a41 = ae[ 3 ], a42 = ae[ 7 ], a43 = ae[ 11 ], a44 = ae[ 15 ];

		const T& b11 = be[ 0 ], b12 = be[ 4 ], b13 = be[ 8 ], b14 = be[ 12 ];
		const T& b21 = be[ 1 ], b22 = be[ 5 ], b23 = be[ 9 ], b24 = be[ 13 ];
		const T& b31 = be[ 2 ], b32 = be[ 6 ], b33 = be[ 10 ], b34 = be[ 14 ];
		const T& b41 = be[ 3 ], b42 = be[ 7 ], b43 = be[ 11 ], b44 = be[ 15 ];

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

	T det() const {
		auto& te = elements;

		const T& n11 = te[ 0 ], n12 = te[ 4 ], n13 = te[ 8 ], n14 = te[ 12 ];
		const T& n21 = te[ 1 ], n22 = te[ 5 ], n23 = te[ 9 ], n24 = te[ 13 ];
		const T& n31 = te[ 2 ], n32 = te[ 6 ], n33 = te[ 10 ], n34 = te[ 14 ];
		const T& n41 = te[ 3 ], n42 = te[ 7 ], n43 = te[ 11 ], n44 = te[ 15 ];

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

	Matrix4& multiplyScalar(T s) {

		auto& te = elements;

		te[ 0 ] *= s; te[ 4 ] *= s; te[ 8 ] *= s; te[ 12 ] *= s;
		te[ 1 ] *= s; te[ 5 ] *= s; te[ 9 ] *= s; te[ 13 ] *= s;
		te[ 2 ] *= s; te[ 6 ] *= s; te[ 10 ] *= s; te[ 14 ] *= s;
		te[ 3 ] *= s; te[ 7 ] *= s; te[ 11 ] *= s; te[ 15 ] *= s;

		return *this;

	}

	std::vector<Vector3<T> > applyToVector3Array(std::vector<Vector3<T> >& array, unsigned offset = 0, unsigned length = 0) {

		Vector3<T> v1;

		if (length == 0) length = array.size();

		for(unsigned i = 0; i < length; i += 3){
			v1.fromArray(array, i + offset);
			v1.applyMatrix4(*this);
			v1.toArray(array, i + offset);
		}

		return *this;

	}

	Matrix4& transpose(){

		auto& te = elements;
		T tmp;

		tmp = te[ 1 ]; te[ 1 ] = te[ 4 ]; te[ 4 ] = tmp;
		tmp = te[ 2 ]; te[ 2 ] = te[ 8 ]; te[ 8 ] = tmp;
		tmp = te[ 6 ]; te[ 6 ] = te[ 9 ]; te[ 9 ] = tmp;

		tmp = te[ 3 ]; te[ 3 ] = te[ 12 ]; te[ 12 ] = tmp;
		tmp = te[ 7 ]; te[ 7 ] = te[ 13 ]; te[ 13 ] = tmp;
		tmp = te[ 11 ]; te[ 11 ] = te[ 14 ]; te[ 14 ] = tmp;

		return *this;

	}

	std::vector<T> flattenToArrayOffset(std::vector<T>& array, unsigned offset) {
		for(unsigned i = 0; i < 16; ++i){
			array[i = offset] = elements[i];
		}
	}

	Matrix4& setPosition(const Vector3<T>& v){
		elements[12] = v.x;
		elements[13] = v.y;
		elements[14] = v.z;

		return *this;
	}

	Matrix4& getInverse(const Matrix4& m, bool throwOnDegenerate = false){

		// based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
		auto& te = elements;
		const auto& me = m.elements;

		const T& n11 = me[ 0 ], n12 = me[ 4 ], n13 = me[ 8 ], n14 = me[ 12 ];
		const T& n21 = me[ 1 ], n22 = me[ 5 ], n23 = me[ 9 ], n24 = me[ 13 ];
		const T& n31 = me[ 2 ], n32 = me[ 6 ], n33 = me[ 10 ], n34 = me[ 14 ];
		const T& n41 = me[ 3 ], n42 = me[ 7 ], n43 = me[ 11 ], n44 = me[ 15 ];

		const T t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44,
				t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44,
				t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44,
				t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

		T det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;

		if ( det == 0 ) {

			const char* msg = "Matrix4.getInverse(): can't invert matrix, determinant is 0";

			if ( throwOnDegenerate ) {

				throw std::runtime_error( msg );

			} else {

				console.warn(msg);

			}

			return identity();

		}

		const T detInv = 1 / det;

		te[ 0 ] = t11 * detInv;
		te[ 1 ] = ( n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44 ) * detInv;
		te[ 2 ] = ( n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44 ) * detInv;
		te[ 3 ] = ( n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43 ) * detInv;

		te[ 4 ] = t12 * detInv;
		te[ 5 ] = ( n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44 ) * detInv;
		te[ 6 ] = ( n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44 ) * detInv;
		te[ 7 ] = ( n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43 ) * detInv;

		te[ 8 ] = t13 * detInv;
		te[ 9 ] = ( n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44 ) * detInv;
		te[ 10 ] = ( n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44 ) * detInv;
		te[ 11 ] = ( n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43 ) * detInv;

		te[ 12 ] = t14 * detInv;
		te[ 13 ] = ( n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34 ) * detInv;
		te[ 14 ] = ( n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34 ) * detInv;
		te[ 15 ] = ( n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33 ) * detInv;

		return *this;

	}

	Matrix4& scale(const Vector3<T>& v){
		auto& te = elements;
		T x = v.x, y = v.y, z = v.z;

		te[ 0 ] *= x; te[ 4 ] *= y; te[ 8 ] *= z;
		te[ 1 ] *= x; te[ 5 ] *= y; te[ 9 ] *= z;
		te[ 2 ] *= x; te[ 6 ] *= y; te[ 10 ] *= z;
		te[ 3 ] *= x; te[ 7 ] *= y; te[ 11 ] *= z;

		return *this;

	}

	T getMaxScaleOnAxis() const {

		auto& te = elements;

		T scaleXSq = te[ 0 ] * te[ 0 ] + te[ 1 ] * te[ 1 ] + te[ 2 ] * te[ 2 ];
		T scaleYSq = te[ 4 ] * te[ 4 ] + te[ 5 ] * te[ 5 ] + te[ 6 ] * te[ 6 ];
		T scaleZSq = te[ 8 ] * te[ 8 ] + te[ 9 ] * te[ 9 ] + te[ 10 ] * te[ 10 ];

		return sqrt( std::max({ scaleXSq, scaleYSq, scaleZSq }) );

	}

	Matrix4& makeTranslation(T x, T y, T z){

		set(
				1, 0, 0, x,
				0, 1, 0, y,
				0, 0, 1, z,
				0, 0, 0, 1
		);

		return *this;

	}

	Matrix4& makeRotationX(T theta){

		T c = cos(theta), s = sin(theta);

		set(
				1, 0, 0, 0,
				0, c, -s, 0,
				0, s, c, 0,
				0, 0, 0, 1
		);

		return *this;

	}
	Matrix4& makeRotationY(T theta){

		T c = cos(theta), s = sin(theta);

		set(
				c, 0, s, 0,
				0, 1, 0, 0,
				-s, 0, c, 0,
				0, 0, 0, 1
		);

		return *this;

	}
	Matrix4& makeRotationZ(T theta){

		T c = cos(theta), s = sin(theta);

		set(
				c, -s, 0, 0,
				s, c, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
		);

		return *this;

	}
	Matrix4& makeRotationAxis(const Vector3<T>& axis, T angle){

		T c = cos(angle);
		T s = sin(angle);
		T t = 1 - c;
		auto& x = axis.x, y= axis.y, z = axis.z;
		T tx = t * x, ty = t * y;

		set(
				tx * x + c, tx * y - s * z, tx * z + s * y, 0,
				tx * y + s * z, ty * y + c, ty * z - s * x, 0,
				tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
				0, 0, 0, 1
		);

		return *this;

	}

	Matrix4& makeScale(T x, T y, T z) {

		set(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
		);

		return *this;

	}

	Matrix4& compose(const Vector3<T>& position, const Quaternion<T>& quaternion, const Vector3<T>& scale) {
		makeRotationFromQuaternion(quaternion);
		this->scale(scale);
		setPosition(position);
		return *this;
	}

	const Matrix4& decompose(Vector3<T>& position, Quaternion<T>& quaternion, Vector3<T>& scale) const {

		Vector3<T> vector;
		Matrix4<T> matrix;

		auto& te = elements;

		T sx = vector.set( te[ 0 ], te[ 1 ], te[ 2 ] ).length();
		T sy = vector.set( te[ 4 ], te[ 5 ], te[ 6 ] ).length();
		T sz = vector.set( te[ 8 ], te[ 9 ], te[ 10 ] ).length();

		// if determine is negative, we need to invert one scale
		T det = this->det();
		if ( det < 0 ) {

			sx = - sx;

		}

		position.x = te[ 12 ];
		position.y = te[ 13 ];
		position.z = te[ 14 ];

		// scale the rotation part

		matrix.copy( *this );

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

//These are defined in a windows header. -_-
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

	Matrix4& makeFrustum(T left, T right, T bottom, T top, T near, T far){

		auto& te = elements;
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

	[[deprecated]]
	Matrix4& makePerspective(T fov, T aspect, T near, T far){

		T ymax = near * tan(Math::degToRad<T>(fov * 0.5));
		T ymin = -ymax;
		T xmin = ymin * aspect;
		T xmax = ymax * aspect;

		return makeFrustum(xmin, xmax, ymin, ymax, near, far);

	}

	Matrix4& makePerspective(T left, T right, T top, T bottom, T near, T far) {

		auto& te = elements;
		T x = 2 * near / (right - left);
		T y = 2 * near / (top - bottom);

		T a = (right + left) / (right - left);
		T b = (top + bottom) / (top - bottom);
		T c = - (far + near) / (far - near);
		T d = - 2 * far * near / (far - near);

		te[ 0 ] = x;	te[ 4 ] = 0;	te[ 8 ] = a;	te[ 12 ] = 0;
		te[ 1 ] = 0;	te[ 5 ] = y;	te[ 9 ] = b;	te[ 13 ] = 0;
		te[ 2 ] = 0;	te[ 6 ] = 0;	te[ 10 ] = c;	te[ 14 ] = d;
		te[ 3 ] = 0;	te[ 7 ] = 0;	te[ 11 ] = - 1;	te[ 15 ] = 0;

		return *this;

	}

	Matrix4& makeOrthographic(T left, T right, T top, T bottom, T near, T far){

		auto& te = elements;
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

	bool equals(const Matrix4& m) const {

		for (unsigned i = 0; i < 16; ++i){

			if (elements[i] == m.elements[i]) return false;
		}

		return true;

	}

	bool operator==(const Matrix4& m) const {
		return equals(m);
	}

	Matrix4& fromArray(const std::vector<T>& array, unsigned offset = 0){

		for (unsigned i = 0; i < 16; ++i){

			elements[i] = array[i + offset];

		}

	}

	std::vector<T>& toArray(std::vector<T>& array, unsigned offset = 0) const {

		for (unsigned i = 0; i < 16; ++i){
			array[i + offset] = elements[i];
		}

		return array;

	}

	/**
	 * This will apply the matrix to a set of vertices already stuffed in a buffer array.
	 *
	 * Offset changes where the loop begins (Defaults to the start of the array)
	 * length changes where the loop ends (Default is the size of the array, despite the default value)
	 */
	std::vector<T>& applyToVector3Array(std::vector<T>& array, unsigned offset = 0, unsigned length = 0) const {

		if (length == 0){
			length = array.size();
		}

		Vector3<T> v1;

		for (unsigned i = offset; i < length; i += 3){
			v1.fromArray(array, i);
			v1.applyMatrix4(*this);
			v1.toArray(array, i);
		}

		return array;

	}

	std::string toString() const {

		std::string r = "[[\n   ";

		for (unsigned i = 0; i < 16; ++i) {
			r += std::to_string(elements[i]) + ", ";
			if ((i + 1) % 4 == 0) r += "]\n [ ";
		}

		r += "]]";

		return r;

	}

};

#endif
