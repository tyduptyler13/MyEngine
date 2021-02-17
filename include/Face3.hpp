#pragma once

#include <memory>
#include <array>

#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Color.hpp"


namespace MyEngine {

	/**
	 * Redesigned to handle max customization. Previously, there were
	 * expandable values for handling the cases where you wanted per
	 * vertex color or normals. Now, those values are always present
	 * and just duplicates of each other if unused. Most geometry these
	 * days is smooth requiring different values for every vertex.
	 *
	 * This memory usage will be optimized before reaching the GPU
	 * using indexing.
	 */
	template<typename T>
	struct Face3 {

		std::array<Vector3<T>, 3> vertices;
		std::array<Vector3<T>, 3> normals;
		std::array<Vector2 < T>, 3>
		uvs;
		std::array<Color, 3> colors;

		unsigned materialIndex = 0;

		Face3() {}

		Face3(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, unsigned materialIndex = 0)
				: materialIndex(materialIndex) {
			vertices[0] = a;
			vertices[1] = b;
			vertices[2] = c;
		}

		Face3(const Face3<T>& f) {
			vertices = f.vertices;
			normals = f.normals;
			colors = f.colors;

			materialIndex = f.materialIndex;
		}

		Face3(Face3&& f) { //Move constructor
			vertices = f.vertices;
			normals = f.normals;
			colors = f.colors;
			materialIndex = f.materialIndex;
		}

		Face3& operator=(const Face3<T>& f) {
			vertices = f.vertices;
			normals = f.normals;
			colors = f.colors;

			materialIndex = f.materialIndex;

			return *this;
		}

		Face3& operator=(Face3&& f) { //Move assignment
			vertices = f.vertices;
			normals = f.normals;
			colors = f.colors;

			materialIndex = f.materialIndex;

			return *this;
		}

	};

}



