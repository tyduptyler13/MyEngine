#ifndef MYUPLAY_MYENGINE_BUFFERGEOMETRY
#define MYUPLAY_MYENGINE_BUFFERGEOMETRY

#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Geometry.hpp"
#include "Math.hpp"

#include <array>
#include <vector>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct BufferGeometry : public virtual IGeometry<T>, public AGeometry<T, BufferGeometry<T>> {

			typedef typename IGeometry<T>::Group Group;

			std::vector<T> vertices;
			std::vector<T> normals;
			std::vector<T> uvs;
			//You will only get indexed performance gains by having a single index buffer
			std::vector<unsigned short> indices;

			std::vector<Group> groups;

			Math::UUID uuid = Math::generateUUID();

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

			std::vector<unsigned short> getIndices() const {
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

				T maxRadiusSq;

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

		};

	}

}

#endif

