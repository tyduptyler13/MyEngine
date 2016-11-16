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
			std::vector<Color> colors;
			std::vector<unsigned> vertexIndices;
			std::vector<unsigned> normalIndices;
			std::vector<unsigned> uvIndices;

			std::vector<Group> groups;

			Math::UUID uuid = Math::generateUUID();

			bool indicesNeedUpdate = true,
					normalsNeedUpdate = true,
					verticesNeedUpdate = true;

			BufferGeometry(){}

			template <typename T2>
			void fromSimpleGeometry(const SimpleGeometry<T2>& geo){

				vertices.clear();
				normals.clear();
				uvs.clear();
				vertexIndices.clear();
				normalIndices.clear();
				uvIndices.clear();

				vertexIndices.reserve(geo.faces.size() * 3);
				normalIndices.reserve(geo.faces.size() * 3);
				uvIndices.reserve(geo.faces.size() * 3);

				indicesNeedUpdate = true;
				normalsNeedUpdate = true;
				verticesNeedUpdate = true;

				for (const Face3<T2>& f : geo.faces){

					for (unsigned v = 0; v < 3; ++v){
						bool matched = false;
						for (unsigned i = 0; i < vertices.size(); ++i){
							if (f.vertices[v] == vertices[i]){
								vertexIndices.push_back(i);
								matched = true;
								break;
							}
						}

						if (!matched){
							vertices.insert(vertices.end(), {f.vertices[v].x, f.vertices[v].y, f.vertices[v].z});
							vertexIndices.push_back( vertices.size() - 1 );
						}
					}

					for (unsigned n = 0; n < 3; ++n){
						bool matched = false;
						for (unsigned i = 0; i < normals.size(); ++i){
							if (f.normals[n] == normals[i]){
								normalIndices.push_back(i);
								matched = true;
								break;
							}
						}

						if (!matched){
							normals.insert(normals.end(), {f.normals[n].x, f.normals[n].y, f.normals[n].z});
							normalIndices.push_back( normals.size() - 1 );
						}
					}

					//TODO uvs;
					//TODO colors;

				}

			}

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

			std::vector<unsigned> getVertexIndices() const {
				return vertexIndices;
			}

			std::vector<T> getNormals() const {
				return normals;
			}

			std::vector<unsigned> getNormalIndices() const {
				return normalIndices;
			}

			std::vector<T> getUVs() const {
				return uvs;
			}

			std::vector<unsigned> getUVIndices() const {
				return uvIndices;
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

			std::vector<Color> getColors() const {
				return colors;
			}

			unsigned size() const {
				return vertices.size();
			}

			bool isMultiMaterial() const {
				return groups.size() > 1;
			}

			std::vector<Group> getGroups() const {
				return groups;
			}

		};

	}

}

#endif

