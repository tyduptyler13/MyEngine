#ifndef MYUPLAY_MYENGINE_BOXGEOMETRY
#define MYUPLAY_MYENGINE_BOXGEOMETRY

#include "Geometry.hpp"
#include "BufferGeometry.hpp"

#include <functional>

namespace MyUPlay {
	namespace MyEngine {

		template <typename T>
		class BoxGeometry : public BufferGeometry<T>, public AGeometry<T, BoxGeometry<T>> {

		private:

			struct Vector { //Anon mini vector;
				T x, y, z;
			};

			static inline unsigned calculateIndexCount(unsigned w, unsigned h, unsigned d) {
				return ((w * h * 2) + (w * d * 2) + (d * h * 2)) * 6;
			}

			static inline unsigned calculateVertexCount(unsigned w, unsigned h, unsigned d) {
				return ((w + 1) * (h + 1) * 2) + ((w + 1) * (d + 1) * 2) + ((d + 1) * (h + 1) * 2);
			}

			void buildPlane(T& u, T& v, T& w, T udir, T vdir, T width, T height, T depth,
					unsigned gridX, unsigned gridY, unsigned materialIndex,
					unsigned& vertexBufferOffset, unsigned& uvBufferOffset, unsigned& indexBufferOffset, unsigned& numberOfVertices, unsigned& groupStart,
					Vector& vector){
				const T segmentWidth = width / gridX;
				const T segmentHeight = height / gridY;

				const T widthHalf = width / 2;
				const T heightHalf = height / 2;
				const T depthHalf = depth / 2;

				const unsigned gridX1 = gridX + 1;
				const unsigned gridY1 = gridY + 1;

				unsigned vertexCounter = 0,
						groupCounter = 0;

				for (unsigned iy = 0; iy < gridY1; ++iy){

					T y = iy * segmentHeight - heightHalf;

					for (unsigned ix = 0; ix < gridX1; ++ix){

						T x = ix * segmentWidth - widthHalf;

						u = x * udir;
						v = y * vdir;
						w = depthHalf;

						this->vertices[vertexBufferOffset] = vector.x;
						this->vertices[vertexBufferOffset + 1] = vector.y;
						this->vertices[vertexBufferOffset + 2] = vector.z;

						u = 0;
						v = 0;
						w = depth > 0 ? 1 : -1;

						this->normals[vertexBufferOffset] = vector.x;
						this->normals[vertexBufferOffset + 1] = vector.y;
						this->normals[vertexBufferOffset + 2] = vector.z;

						this->uvs[uvBufferOffset] = T(ix) / gridX;
						this->uvs[uvBufferOffset + 1] = 1 - T(iy) / gridY;

						vertexBufferOffset += 3;
						uvBufferOffset += 2;
						vertexCounter += 1;

					}

				}

				for (unsigned iy = 0; iy < gridY; ++iy) {

					for (unsigned ix = 0; ix < gridX; ++ix) {

						unsigned a = numberOfVertices + ix + gridX1 * iy,
								b = numberOfVertices + ix + gridX1 * ( iy + 1 ),
								c = numberOfVertices + ( ix + 1 ) + gridX1 * ( iy + 1 ),
								d = numberOfVertices + ( ix + 1 ) + gridX1 * iy;

						this->vertexIndices[ indexBufferOffset ] = a;
						this->vertexIndices[ indexBufferOffset + 1] = b;
						this->vertexIndices[ indexBufferOffset + 2] = d;

						this->vertexIndices[ indexBufferOffset + 3] = b;
						this->vertexIndices[ indexBufferOffset + 4] = c;
						this->vertexIndices[ indexBufferOffset + 5] = d;

						indexBufferOffset += 6;
						groupCounter += 6;

					}

				}

				this->addGroup(groupStart, groupCounter, materialIndex);

				groupStart += groupCounter;

				numberOfVertices += vertexCounter;

			}

		public:

			BoxGeometry(T width, T height, T depth, unsigned widthSegments = 1, unsigned heightSegments = 1, unsigned depthSegments = 1) {

				const unsigned vertexCount = calculateVertexCount(widthSegments, heightSegments, depthSegments);
				const unsigned indexCount = calculateIndexCount(widthSegments, heightSegments, depthSegments);

				this->vertexIndices.resize(indexCount);
				this->vertices.resize(vertexCount * 3);
				this->normals.resize(vertexCount * 3);
				this->uvs.resize(vertexCount * 2);

				unsigned vertexBufferOffset = 0,
						uvBufferOffset = 0,
						indexBufferOffset = 0,
						numberOfVertices = 0;

				unsigned groupStart = 0;

				Vector vector;

				//TODO Improve the call structure of this function
				buildPlane( vector.z, vector.y, vector.x, -1, -1, depth, height,  width, depthSegments, heightSegments, 0, vertexBufferOffset, uvBufferOffset, indexBufferOffset, numberOfVertices, groupStart, vector ); // px
				buildPlane( vector.z, vector.y, vector.x,  1, -1, depth, height, -width,  depthSegments, heightSegments, 1, vertexBufferOffset, uvBufferOffset, indexBufferOffset, numberOfVertices, groupStart, vector ); // nx
				buildPlane( vector.x, vector.z, vector.y,  1,  1, width, depth,   height, widthSegments, depthSegments,  2, vertexBufferOffset, uvBufferOffset, indexBufferOffset, numberOfVertices, groupStart, vector ); // py
				buildPlane( vector.x, vector.z, vector.y,  1, -1, width, depth,  -height, widthSegments, depthSegments,  3, vertexBufferOffset, uvBufferOffset, indexBufferOffset, numberOfVertices, groupStart, vector ); // ny
				buildPlane( vector.x, vector.y, vector.z,  1, -1, width, height,  depth,  widthSegments, heightSegments, 4, vertexBufferOffset, uvBufferOffset, indexBufferOffset, numberOfVertices, groupStart, vector ); // pz
				buildPlane( vector.x, vector.y, vector.z, -1, -1, width, height, -depth,  widthSegments, heightSegments, 5, vertexBufferOffset, uvBufferOffset, indexBufferOffset, numberOfVertices, groupStart, vector ); // nz

			}

		};

	}
}

#endif
