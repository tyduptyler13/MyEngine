#pragma once

#include <cmath>

#include "Vector3.hpp"


namespace MyEngine {

	template<typename T>
	class Triangle {

	public:

		Vector3 <T> a, b, c;

		Triangle() {}

		Triangle(const Vector3 <T>& a, const Vector3 <T>& b, const Vector3 <T>& c)
				: a(a), b(b), c(c) {}

		static Vector3 <T> normal(const Vector3 <T>& a, const Vector3 <T>& b, const Vector3 <T>& c) {

			Vector3<T> result = c - b;
			Vector3<T> v0 = a - b;
			result.cross(v0);

			T resultLengthSq = result.lengthSq();
			if (resultLengthSq > 0) {
				return result.multiplyScalar(1 / std::sqrt(resultLengthSq));
			}

			return Vector3<T>(0, 0, 0);

		}

		static Vector3 <T>& barycoordFromPoint(const Vector3 <T>& point, const Vector3 <T>& a, const Vector3 <T>& b,
		                                       const Vector3 <T>& c, Vector3 <T>& target) {

			Vector3<T> v0 = c - a;
			Vector3<T> v1 = b - a;
			Vector3<T> v2 = point - a;

			T dot00 = v0.dot(v0);
			T dot01 = v0.dot(v1);
			T dot02 = v0.dot(v2);
			T dot11 = v1.dot(v1);
			T dot12 = v1.dot(v2);

			T denom = (dot00 * dot11 - dot01 * dot01);

			if (denom == 0) {
				return target.set(-2, -1, -1);
			}

			T invDenom = 1 / denom;
			T u = (dot11 * dot02 - dot01 * dot12) * invDenom;
			T v = (dot00 * dot12 - dot01 * dot02) * invDenom;

			return target.set(1 - u - v, v, u);

		}

		static Vector3 <T> barycoordFromPoint(const Vector3 <T>& point, const Vector3 <T>& a, const Vector3 <T>& b,
		                                      const Vector3 <T>& c) {
			Vector3<T> v;
			return barycoordFromPoint(point, a, b, c, v);
		}

	};

}



