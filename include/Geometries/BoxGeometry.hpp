#pragma once

#include "BufferGeometry.hpp"

#include <functional>


namespace MyEngine {

	template<typename T>
	class BoxGeometry : public BufferGeometry<T> {

	private:

		struct Vector { //Anon mini vector;
			T x, y, z;
		};

	public:

		BoxGeometry(T width, T height, T depth, unsigned widthSegments, unsigned heightSegments, unsigned depthSegments) {

			std::vector<unsigned int> indices;
			std::vector<T> vertices;
			std::vector<T> normals;
			std::vector<T> uvs;

			unsigned numberOfVertices = 0;
			unsigned groupStart = 0;

			Vector vector;

			auto buildPlane = [&](T& u, T& v, T& w, T udir, T vdir, T width, T height, T depth, unsigned gridX,
			                      unsigned gridY, unsigned materialIndex) {
				const T segmentWidth = width / gridX;
				const T segmentHeight = height / gridY;

				const T widthHalf = width / 2;
				const T heightHalf = height / 2;
				const T depthHalf = depth / 2;

				const unsigned gridX1 = gridX + 1;
				const unsigned gridY1 = gridY + 1;

				unsigned vertexCounter = 0,
						groupCounter = 0;

				for (unsigned iy = 0; iy < gridY1; ++iy) {

					T y = iy * segmentHeight - heightHalf;

					for (unsigned ix = 0; ix < gridX1; ++ix) {

						T x = ix * segmentWidth - widthHalf;

						u = x * udir;
						v = y * vdir;
						w = depthHalf;

						vertices.insert(vertices.end(), {vector.x, vector.y, vector.z});

						u = 0;
						v = 0;
						w = depth > 0 ? 1 : -1;

						normals.insert(normals.end(), {vector.x, vector.y, vector.z});

						uvs.insert(uvs.end(), {(T) ix / gridX, (T) iy / gridY});

						vertexCounter++;

					}

				}

				for (unsigned iy = 0; iy < gridY; ++iy) {

					for (unsigned ix = 0; ix < gridX; ++ix) {

						unsigned a = numberOfVertices + ix + gridX1 * iy,
								b = numberOfVertices + ix + gridX1 * (iy + 1),
								c = numberOfVertices + (ix + 1) + gridX1 * (iy + 1),
								d = numberOfVertices + (ix + 1) + gridX1 * iy;

						//Vertices
						indices.insert(indices.end(), {a, b, d, b, c, d});

						groupCounter += 6;

					}

				}

				this->addGroup(groupStart, groupCounter, materialIndex);

				groupStart += groupCounter;

				numberOfVertices += vertexCounter;

			}; //buildPlane

			buildPlane(vector.z, vector.y, vector.x, -1, -1, depth, height, width, depthSegments, heightSegments, 0); // px
			buildPlane(vector.z, vector.y, vector.x, 1, -1, depth, height, -width, depthSegments, heightSegments, 1); // nx
			buildPlane(vector.x, vector.z, vector.y, 1, 1, width, depth, height, widthSegments, depthSegments, 2); // py
			buildPlane(vector.x, vector.z, vector.y, 1, -1, width, depth, -height, widthSegments, depthSegments, 3); // ny
			buildPlane(vector.x, vector.y, vector.z, 1, -1, width, height, depth, widthSegments, heightSegments, 4); // pz
			buildPlane(vector.x, vector.y, vector.z, -1, -1, width, height, -depth, widthSegments, heightSegments, 5); // nz

			this->positions = vertices;
			this->normals = normals;
			this->uvs = uvs;
			this->indices = indices;

		}

		BoxGeometry(T width, T height, T depth) : BoxGeometry(width, depth, height, 1, 1, 1) {}

	};

}


