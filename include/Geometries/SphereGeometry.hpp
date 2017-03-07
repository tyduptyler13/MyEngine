#ifndef MYUPLAY_MYENGINE_SPHEREGEOMETRY
#define MYUPLAY_MYENGINE_SPHEREGEOMETRY

#include <vector>

#include "BufferGeometry.hpp"
#include "Math.hpp"

namespace MyUPlay {
	namespace MyEngine {

		template <typename T>
		struct SphereGeometry : public BufferGeometry<T> {

			SphereGeometry(T radius, unsigned widthSegments, unsigned heightSegments,
					T phiStart, T phiLength = Math::PI * 2, T thetaStart = 0, T thetaLength = Math::PI){

				widthSegments = std::max<unsigned>(3, widthSegments);
				heightSegments = std::max<unsigned>(2, heightSegments);

				const T thetaEnd = thetaStart + thetaLength;

				unsigned index = 0;
				std::vector<std::vector<unsigned>> grid;

				Vector3<T> vertex;
				Vector3<T> normal;

				for (unsigned iy = 0; iy <= heightSegments; ++iy){

					std::vector<unsigned> verticesRow;

					T v = (T) iy / heightSegments;

					for (unsigned ix = 0; ix <= widthSegments; ++ix){

						T u = (T) ix / widthSegments;

						vertex.x = -radius * std::cos(phiStart + u * phiLength) * std::sin(thetaStart + v * thetaLength);
						vertex.y = radius * std::cos(thetaStart + v * thetaLength);
						vertex.z = radius * std::sin(phiStart + u * phiLength) * std::sin(thetaStart + v * thetaLength);

						this->vertices.insert(this->vertices.end(), {vertex.x, vertex.y, vertex.z});

						normal.set(vertex.x, vertex.y, vertex.z).normalize();
						this->normals.insert(this->normals.end(), {normal.x, normal.y, normal.z});

						this->uvs.insert(this->uvs.end(), {u, 1 - v});

						verticesRow.push_back(index++);

					}

					grid.push_back(verticesRow);

				}

				//Indices

				for (unsigned iy = 0; iy < heightSegments; ++iy){
					for (unsigned ix = 0; ix < widthSegments; ++ix){
						unsigned a = grid[iy][ix+1],
								b = grid[iy][ix],
								c = grid[iy+1][ix],
								d = grid[iy+1][ix+1];

						if (iy != 0 || thetaStart > 0) this->indices.insert(this->indices.end(), {a, b, d});
						if (iy != heightSegments - 1 || thetaEnd < Math::PI) this->indices.insert(this->indices.end(), {b, c, d});

					}
				}

				this->indicesNeedUpdate = true;

			}

			//Explicit constructors for js bindings.

			SphereGeometry(T radius, unsigned widthSegments, unsigned heightSegments = 6) : SphereGeometry(radius, widthSegments, heightSegments, 0) {}

			SphereGeometry(T radius) : SphereGeometry(radius, 8) {}

			SphereGeometry(const SphereGeometry& s) : BufferGeometry<T>(s) {}

		};

	}
}

#endif
