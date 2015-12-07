#ifndef MYUPLAY_MYENGINE_GEOMETRY
#define MYUPLAY_MYENGINE_GEOMETRY

#include <vector>
#include <string>
#include <memory>

#include "EventDispatcher.hpp"
#include "Math.hpp"
#include "Vector3.hpp"
#include "Color.hpp"
#include "Face3.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"
#include "Box3.hpp"
#include "Sphere.hpp"
#include "Matrix3.hpp"
#include "Matrix4.hpp"
#include "BufferGeometry.hpp"
#include "Mesh.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct MorphTargets {
			std::string name;
			std::vector<Vector3<T> > vertices;
		};

		template <typename T>
		struct MorphNormals {
			std::string name;
			std::vector<Vector3<T> > normals;
		};

		template <typename T>
		class Geometry : public EventDispatcher<Geometry<T>, std::string> {

			Math::UUID uuid = Math::generateUUID();

			std::string name;

			std::vector<Vector3<T> > vertices;
			std::vector<Color> colors;
			std::vector<Face3<T> > faces;
			std::vector<std::vector<Vector2<T> > > faceVertexUvs;

			std::vector<MorphTargets<T> > morphTargets;
			std::vector<MorphNormals<T> > morphNormals;

			std::vector<Vector4<T> > skinWeights;
			std::vector<Vector4<T> > skinIndices;

			std::vector<T> lineDistances;

			std::unique_ptr<Box3<T> > boundingBox = NULL;
			std::unique_ptr<Sphere<T> > boundingSphere = NULL;

			bool verticesNeedUpdate = false,
			     elementsNeedUpdate = false,
			     uvsNeedUpdate = false,
			     normalsNeedUpdate = false,
			     colorsNeedUpdate = false,
			     lineDistancesNeedUpdate = false,
			     groupsNeedUpdate = false;

			void applyMatrix(const Matrix4<T>& matrix);

			Geometry& rotateX(T angle) {
				Matrix4<T> m1;
				m1.makeRotationX(angle);
				applyMatrix(m1);
				return *this;
			}

			Geometry& rotateY(T angle) {
				Matrix4<T> m1;
				m1.makeRotationY(angle);
				applyMatrix(m1);
				return *this;
			}

			Geometry& rotateZ(T angle) {
				Matrix4<T> m1;
				m1.makeRotationZ(angle);
				applyMatrix(angle);
				return *this;
			}

			Geometry& translate(T x, T y, T z) {
				Matrix4<T> m1;
				m1.makeTranslation(x, y, z);
				applyMatrix(m1);
				return *this;
			}

			Geometry& scale(T x, T y, T z) {
				Matrix4<T> m1;
				m1.makeScale(x, y, z);
				applyMatrix(m1);
				return *this;
			}

			Geometry& lookAt(const Vector3<T>& v){
				Matrix4<T> m1;
				m1.lookAt(v);
				applyMatrix(m1);
				return *this;
			}

			Geometry& fromBufferGeometry(const BufferGeometry<T>& geometry);

			Vector3<T> center(){
				computeBoundingBox();
				Vector3<T> offset = boundingBox.center().negate();
				translate(offset.x, offset.y, offset.z);
				return offset;
			}

			Geometry& normalize();

			void computeFaceNormals();
			void computeVertexNormals(bool areaWeighted);
			void computeMorphNormals();

			void computeLineDistances(){

				T d = 0;

				for (unsigned i = 0; i < vertices.size(); ++i){

					if (i > 0){
						d += vertices[i].distanceTo(vertices[i - 1]);
					}

					lineDistances[i] = d;

				}

			}

			void computeBoundingBox(){
				if (boundingBox == NULL){
					boundingBox = new Box3<T>();
				}
				boundingBox.setFromPoints(vertices);
			}

			void computeBoundingSphere(){
				if (boundingSphere == NULL){
					boundingSphere = new Sphere<T>();
				}
				boundingSphere.setFromPoints(vertices);
			}

			void merge(const Geometry& geometry, const Matrix4<T>& matrix, int materialIndexOffset);

			void mergeMesh(const Mesh<T>& mesh);

			unsigned mergeVertices();
			void sortFacesByMaterialIndex();

			Geometry(const Geometry& geometry){
				copy(geometry);
			}

			Geometry& copy(const Geometry& geometry);
			Geometry& operator=(const Geometry& geometry){
				copy(geometry);
			}

			void dispose() const {
				dispatchEvent("dispose", *this);
			}

		};

	}

}

#endif

