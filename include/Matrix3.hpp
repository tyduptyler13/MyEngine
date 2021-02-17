#pragma once

#include <vector>
#include <array>
#include <string>
#include <stdexcept>
#include <iostream>


namespace MyEngine {
	template<typename T>
	struct Matrix3;
	typedef Matrix3<float> Matrix3f;
	typedef Matrix3<double> Matrix3d;
}


#include "Vector3.hpp"
#include "Matrix4.hpp"

template<typename T = float>
struct MyEngine::Matrix3 {

	//Column major notation.
	std::array<T, 9> elements = {{
			                             1, 0, 0, //First column
			                             0, 1, 0, //Second column
			                             0, 0, 1
	                             }};

	Matrix3() {}

	Matrix3(const Matrix3& m) {

		for (unsigned i = 0; i < 9; ++i) {

			elements[i] = m.elements[i];

		}

	}

	Matrix3& operator=(const Matrix3& m) {
		for (unsigned i = 0; i < 9; ++i) {
			elements[i] = m.elements[i];
		}
	}

	Matrix3& set(T n11, T n12, T n13, T n21, T n22, T n23, T n31, T n32, T n33) {

		auto& te = elements;

		te[0] = n11;
		te[3] = n12;
		te[6] = n13;
		te[1] = n21;
		te[4] = n22;
		te[7] = n23;
		te[2] = n31;
		te[5] = n32;
		te[8] = n33;

		return *this;

	}

	Matrix3& identity() {
		set(
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
		);

		return *this;
	}

	std::vector<Vector3<T> >& applyToVector3Array(std::vector<Vector3<T> >& array, unsigned offset = 0,
	                                              unsigned length = 0) const {

		Vector3 <T> v1;

		if (length == 0) length = array.size();

		for (unsigned i = 0; i < length - offset; i += 3) {
			v1.fromArray(array, i);
			v1.applyMatrix3(*this);
			v1.toArray(array, i);
		}

		return array;

	}

	Matrix3& multiply(T s) {
		for (unsigned i = 0; i < 9; ++i) {
			elements[i] *= s;
		}
		return *this;
	}

	Matrix3 operator*(T s) {
		return Matrix3(*this).multiply(s);
	}

	Matrix3& operator*=(T s) {
		return multiply(s);
	}

	T det() const {

		T& a = elements[0], b = elements[1], c = elements[2],
				d = elements[3], e = elements[4], f = elements[5],
				g = elements[6], h = elements[7], i = elements[8];

		return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;

	}

	Matrix3& getInverse(const Matrix4<T>& m, bool throwOnDegenerate = false) {

		auto& me = m.elements;
		auto& te = elements;

		te[0] = me[10] * me[5] - me[6] * me[9];
		te[1] = -me[10] * me[1] + me[2] * me[9];
		te[2] = me[6] * me[1] - me[2] * me[5];
		te[3] = -me[10] * me[4] + me[6] * me[8];
		te[4] = me[10] * me[0] - me[2] * me[8];
		te[5] = -me[6] * me[0] + me[2] * me[4];
		te[6] = me[9] * me[4] - me[5] * me[8];
		te[7] = -me[9] * me[0] + me[1] * me[8];
		te[8] = me[5] * me[0] - me[1] * me[4];

		T det = me[0] * te[0] + me[1] * te[3] + me[2] * te[6];

		//no inverse

		if (det == 0) {

			const std::string msg = "Matrix3.getInverse(): can't invert matrix, determinant is 0";

			if (throwOnDegenerate) {
				throw std::logic_error(msg);
			} else {
				std::cerr << msg << std::endl;
			}

			identity();

			return *this;

		}

		multiply(1.0 / det);

		return *this;

	}

	Matrix3& transpose() {
		T tmp;
		auto& m = elements;

		tmp = m[1];
		m[1] = m[3];
		m[3] = tmp;
		tmp = m[2];
		m[2] = m[6];
		m[6] = tmp;
		tmp = m[5];
		m[5] = m[7];
		m[7] = tmp;

		return *this;
	}

	std::vector<T> flattenToArrayOffset(std::vector<T>& array, unsigned offset) const {

		array.reserve(offset + 8);

		for (unsigned i = 0; i < 9; ++i) {
			array[offset + i] = elements[i];
		}

	}

	Matrix3& getNormalMatrix(const Matrix4<T>& m) {

		return getInverse(m).transpose();

	}

	Matrix3& transposeIntoArray(std::vector<T>& r) const {

		auto& m = elements;

		r[0] = m[0];
		r[1] = m[3];
		r[2] = m[6];
		r[3] = m[1];
		r[4] = m[4];
		r[5] = m[7];
		r[6] = m[2];
		r[7] = m[5];
		r[8] = m[8];

		return *this;

	}

	/**
	 * This will apply the matrix to a set of vertices already stuffed in a buffer array.
	 *
	 * Offset changes where the loop begins (Defaults to the start of the array)
	 * length changes where the loop ends (Default is the size of the array, despite the default value)
	 */
	std::vector<T>& applyToVector3Array(std::vector<T>& array, unsigned offset = 0, unsigned length = 0) const {

		if (length == 0) {
			length = array.size();
		}

		Vector3 <T> v1;

		for (unsigned i = offset; i < length; i += 3) {
			v1.fromArray(array, i);
			v1.applyMatrix3(*this);
			v1.toArray(array, i);
		}

		return array;

	}

	Matrix3& fromArray(const std::array<T, 9>& array) {
		elements = array;
		return *this;
	}

	std::array<T, 9> toArray() const {

		return elements; //This will be copied.

	}

};


