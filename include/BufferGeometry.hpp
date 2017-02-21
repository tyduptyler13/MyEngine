#ifndef MYUPLAY_MYENGINE_BUFFERGEOMETRY
#define MYUPLAY_MYENGINE_BUFFERGEOMETRY

#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Geometry.hpp"
#include "Math.hpp"

#include <array>
#include <vector>
#include <memory>

namespace MyUPlay {

	namespace MyEngine {

		/**
		 * BufferGeometry holds all important information in indexed buffer arrays.
		 */
		template <typename T>
		struct BufferGeometry : public IGeometry<T> {

			typedef typename IGeometry<T>::Group Group;

			std::vector<T> vertices;
			std::vector<T> normals;
			std::vector<T> uvs;
			// Colors are not indexed. Doing so would result in a waste of memory space/bandwidth regardless of indexing
			// This is because an index stores one set of each value, if a color is the only thing different, it still
			// must be duplicated, even if 99% of everything else is the same.
			std::vector<unsigned char> colors;
			//You will only get indexed performance gains by having a single index buffer
			std::vector<unsigned int> indices;

			std::vector<Group> groups;

			BufferGeometry(){}

			//TODO Add optimization method

			void addGroup(unsigned start, unsigned count, unsigned materialIndex) {
				groups.emplace_back(Group(start, count, materialIndex));
			}

			void applyMatrix(const Matrix4<T>& matrix){
				matrix.applyToVector3Array(vertices);
				Matrix3<T> m3;
				m3.getNormalMatrix(matrix).applyToVector3Array(normals);
				this->computeBoundingBox();
				this->computeBoundingSphere();
			}

			std::vector<T> getVertices() const {
				return vertices;
			}

			std::vector<T> getNormals() const {
				return normals;
			}

			std::vector<unsigned int> getIndices() const {
				return indices;
			}

			void computeBoundingBox(){

				if (!this->boundingBoxDirty) return;

				if (this->boundingBox == nullptr){
					this->boundingBox = std::make_unique<Box3<T>>();
				}

				this->boundingBox->setFromArray(vertices);

			}

			void computeBoundingSphere() {

				if (!this->boundingSphereDirty) return;

				if (this->boundingSphere == nullptr){
					this->boundingSphere = std::make_unique<Sphere<T>>();
				}

				computeBoundingBox();

				this->boundingBox->center(this->boundingSphere->center);

				//The radius doesn't necessarily include the corners of the bounding box.

				T maxRadiusSq = 0;

				Vector3<T> vertex;

				for (unsigned i = 0; i < vertices.size(); ++i){
					//We thus check every vertex for the max distance from the center.
					vertex.fromArray(vertices, i);
					maxRadiusSq = std::max(maxRadiusSq, this->boundingSphere->center.distanceToSquared(vertex));
				}

				this->boundingSphere->radius = std::sqrt(maxRadiusSq);

			}

			unsigned size() const {
				return indices.size();
			}

			std::vector<Group> getGroups() const {
				return groups;
			}

			void raycast(std::shared_ptr<Object3D<T>>& obj, const Raycaster<T>& r, std::vector<Intersection<T>>& intersections, SideConstant s) const {

				for (unsigned offset = 0; offset < indices.size(); offset +=3) {

					unsigned a = indices[offset], b = indices[offset + 1], c = indices[offset + 2];

					Vector3<T> vA, vB, vC;

					vA.fromArray(vertices, a);
					vB.fromArray(vertices, b);
					vC.fromArray(vertices, c);

					Vector2<T> uvA, uvB, uvC;

					uvA.fromArray(uvs, a);
					uvB.fromArray(uvs, b);
					uvC.fromArray(uvs, c);

					auto intersection = r.checkIntersection(obj, s, vA, vB, vC);

					if (intersection) {
						if (uvs.size() > 0) {
							intersection->uv = Raycaster<T>::uvIntersection(intersection->point, vA, vB, vC, uvA, uvB, uvC);
						}

						//TODO Improve face constructor to have all of the data. (Color, uvs, verts, etc)
						//intersection->face;

						intersections.push_back(*intersection);

					}

				}

			}

		};

	}

}

#endif

