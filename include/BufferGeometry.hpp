#ifndef MYUPLAY_MYENGINE_BUFFERGEOMETRY
#define MYUPLAY_MYENGINE_BUFFERGEOMETRY

#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Geometry.hpp"
#include "Math.hpp"
#include "BufferAttribute.hpp"

#include <array>
#include <vector>
#include <unordered_map>
#include <memory>

namespace MyUPlay {

	namespace MyEngine {

		/**
		 * BufferGeometry holds all important information in indexed buffer arrays.
		 */
		template <typename T>
		struct BufferGeometry : public IGeometry<T> {

			typedef typename IGeometry<T>::Group Group;

			BufferAttribute<T> positions;
			BufferAttribute<T> normals;
			BufferAttribute<T> uvs;

			BufferAttribute<unsigned char> color;

			//You will only get indexed performance gains by having a single index buffer
			BufferAttribute<unsigned int> indices;

			std::vector<Group> groups;

			BufferGeometry() : positions(3), normals(3), uvs(2), color(4){
				positions.onUpdate([this](){
					this->boundingBoxDirty = true;
					this->boundingSphereDirty = true;
				});
			}

			//TODO Add optimization method

			void addGroup(unsigned start, unsigned count, unsigned materialIndex) {
				groups.emplace_back(Group(start, count, materialIndex));
			}

			void applyMatrix(const Matrix4<T>& matrix){
				positions.getBuffer([&](std::vector<T>& buf){
					matrix.applyToVector3Array(buf);
				});
				normals.getBuffer([&](std::vector<T>& buf){
					Matrix3<T> m3;
					m3.getNormalMatrix(matrix).applyToVector3Array(buf);
				});
				this->computeBoundingBox();
				this->computeBoundingSphere();
			}

			const BufferAttribute<unsigned int>& getIndexBuffer() const {
				return indices;
			}

			void computeBoundingBox(){

				if (!this->boundingBoxDirty) return;

				if (this->boundingBox == nullptr){
					this->boundingBox = std::make_unique<Box3<T>>();
				}

				this->boundingBox->setFromArray(positions);

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

				for (unsigned i = 0; i < positions.size(); ++i){
					//We thus check every vertex for the max distance from the center.
					vertex.fromArray(positions, i);
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

			bool isBufferGeometry() const {
				return true;
			}

			BufferAttribute<T>& getPositions() {
				return positions;
			}

			BufferAttribute<T>& getNormals() {
				return normals;
			}

			BufferAttribute<unsigned int>& getIndices() {
				return indices;
			}

			void raycast(std::shared_ptr<Object3D<T>>& obj, const Raycaster<T>& r, std::vector<Intersection<T>>& intersections, SideConstant s) const {

				for (unsigned offset = 0; offset < indices.size(); offset +=3) {

					unsigned a = indices[offset], b = indices[offset + 1], c = indices[offset + 2];

					Vector3<T> vA, vB, vC;

					vA.fromArray(positions, a);
					vB.fromArray(positions, b);
					vC.fromArray(positions, c);

					Vector2<T> uvA, uvB, uvC;

					uvA.fromArray(uvs, a);
					uvB.fromArray(uvs, b);
					uvC.fromArray(uvs, c);

					auto intersection = r.checkIntersection(obj, s, vA, vB, vC);

					if (intersection) {
						if (uvs.size() > 0) {
							intersection->uvs = Raycaster<T>::uvIntersection(intersection->point, vA, vB, vC, uvA, uvB, uvC);
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

