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

		struct IndexedFace {

			//All internal attributes reference indexed values.
			std::array<unsigned, 3> vertices;
			std::array<unsigned, 3> normals;
			std::array<unsigned, 3> uvs;
			std::array<Color, 3> colors;

		};



		template <typename T>
		struct BufferGeometry : public IGeometry<T>, public AGeometry<T, BufferGeometry<T>> {

			struct Group {
				unsigned start,
				count,
				materialIndex;
			};

			std::vector<T> vertices;
			std::vector<T> normals;
			std::vector<T> uvs;
			std::vector<unsigned> indices;

			std::vector<Group> groups;

			Math::UUID uuid = Math::generateUUID();

			std::vector<IndexedFace> faces;

			bool indicesNeedUpdate = true,
					normalsNeedUpdate = true,
					verticesNeedUpdate = true;

			BufferGeometry(){}

			template <typename T2>
			void fromSimpleGeometry(const SimpleGeometry<T2>& geo){

				std::vector<Vector3<T>> vertices;
				std::vector<Vector3<T>> normals;
				std::vector<Vector2<T>> uvs;

				vertices.clear();
				normals.clear();
				faces.clear();

				indicesNeedUpdate = true;
				normalsNeedUpdate = true;
				verticesNeedUpdate = true;

				for (const Face3<T2>& f : geo.faces){

					IndexedFace face;

					for (unsigned v = 0; v < 3; ++v){
						bool matched = false;
						for (unsigned i = 0; i < vertices.size(); ++i){
							if (f.vertices[v] == vertices[i]){
								face.vertices[v] = i;
								matched = true;
								break;
							}
						}

						if (!matched){
							vertices.push_back(f.vertices[v]);
							face.vertices[v] = vertices.size() - 1;
						}
					}

					for (unsigned n = 0; n < 3; ++n){
						bool matched = false;
						for (unsigned i = 0; i < normals.size(); ++i){
							if (f.normals[n] == normals[i]){
								face.normals[n] = i;
								matched = true;
								break;
							}
						}

						if (!matched){
							normals.push_back(f.normals[n]);
							face.normals[n] = normals.size() - 1;
						}
					}

					//TODO uvs;

					//TODO convert the arrays to raw type arrays Vec3 -> floats

					face.colors = f.colors;

					faces.push_back(face);

				}

			}

			void addGroup(unsigned start, unsigned count, unsigned materialIndex) {

				groups.emplace_back(Group(start, count, materialIndex));

			}

		};

	}

}

#endif

