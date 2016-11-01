#ifndef MYUPLAY_MYENGINE_GEOMETRY
#define MYUPLAY_MYENGINE_GEOMETRY

#include <vector>
#include <string>
#include <memory>
#include <string>
#include <unordered_map>
#include <array>
#include <algorithm>

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
		struct Geometry {

			Math::UUID uuid = Math::generateUUID();

			std::string name;

			std::vector<Face3<T> > faces;

			/* Disabled until it is used.
			std::vector<MorphTargets<T> > morphTargets;
			std::vector<MorphNormals<T> > morphNormals;

			std::vector<Vector4<T> > skinWeights;
			std::vector<Vector4<T> > skinIndices;

			std::vector<T> lineDistances;
			 */

			std::unique_ptr<Box3<T> > boundingBox;
			std::unique_ptr<Sphere<T> > boundingSphere;

			/**
			 * This is a pointer to a indexed geometry. If it doesn't exist, it
			 * will be created at render time. Setting flags to dirty here will
			 * trigger updates in the buffer geometry.
			 */
			std::unique_ptr<BufferGeometry<T>> optimizedGeometry;

			bool verticesNeedUpdate = false,
					elementsNeedUpdate = false,
					uvsNeedUpdate = false,
					normalsNeedUpdate = false,
					colorsNeedUpdate = false,
					lineDistancesNeedUpdate = false,
					groupsNeedUpdate = false;

			void applyMatrix(const  Matrix4<T>& matrix){
				Matrix3<T> normalMatrix = Matrix3<T>().getNormalMatrix( matrix );

				for (Face3<T>& f : faces){

					for (Vector3<T>& v : f.vertices){
						v = v.applyMatrix4(matrix);
					}

					for (Vector3<T>& n : f.normals){
						n = n.applyMatrix3(normalMatrix).normalize();
					}

				}

				if ( boundingBox != NULL ) {

					computeBoundingBox();

				}

				if ( boundingSphere != NULL ) {

					computeBoundingSphere();

				}

				verticesNeedUpdate = true;
				normalsNeedUpdate = true;

			}

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

			Geometry& fromBufferGeometry(const BufferGeometry<T>& geometry); //TODO

			Vector3<T> center(){
				computeBoundingBox();
				Vector3<T> offset = boundingBox.center().negate();
				translate(offset.x, offset.y, offset.z);
				return offset;
			}

			Geometry& normalize(){
				computeBoundingSphere();

				Vector3<T> center = boundingSphere.center;
				T radius = boundingSphere.radius;

				T s = radius == 0 ? 1 : 1.0 / radius;

				Matrix4<T> matrix;
				matrix.set(
						s, 0, 0, - s * center.x,
						0, s, 0, - s * center.y,
						0, 0, s, - s * center.z,
						0, 0, 0, 1
				);

				applyMatrix( matrix );

				return *this;
			}

			void computeFaceNormals(){
				Vector3<T> cb;
				Vector3<T> ab;

				for (Face3<T>& face : faces) {

					Vector3<T>& vA = face.vertices[0];
					Vector3<T>& vB = face.vertices[1];
					Vector3<T>& vC = face.vertices[2];

					cb.subVectors( vC, vB );
					ab.subVectors( vA, vB );
					cb.cross( ab );

					cb.normalize();

					face.normals.fill(cb);

				}
			}

			void computeVertexNormals(bool areaWeighted = true){

				std::vector<Vector3<T>> tmp;
				tmp.reserve(faces.size() * 3);

				if ( areaWeighted ) {

					// vertex normals weighted by triangle areas
					// http://www.iquilezles.org/www/articles/normals/normals.htm

					Vector3<T> cb;
					Vector3<T> ab;

					for (Face3<T>& face : faces) {

						Vector3<T>& vA = face.vertices[0];
						Vector3<T>& vB = face.vertices[1];
						Vector3<T>& vC = face.vertices[2];

						cb.subVectors( vC, vB );
						ab.subVectors( vA, vB );
						cb.cross( ab );

						tmp.insert(tmp.end(), {cb, cb, cb});

					}

				} else {

					computeFaceNormals();

					for (Face3<T>& face : faces){
						tmp.insert(tmp.end(), face.normals);
					}

				}

				for (Vector3<T>& v : tmp){
					v.normalize();
				}

				for (int i = 0; i < faces.size(); ++i){

					Face3<T>& face = faces[i];

					face.normals = { tmp[i*3], tmp[i*3 + 1], tmp[i*3 + 2] };

				}

			}

			void computeFlatVertexNormals(){

				computeFaceNormals();

			}

			void computeMorphNormals(); //TODO Reimplement morph targets.


			void computeBoundingBox(){
				if (boundingBox == NULL){
					boundingBox = new Box3<T>();
				}
				std::vector<Vector3<T>> vertices;
				vertices.reserve(faces.size() * 3);
				for (Face3<T>& face : faces){
					vertices.insert(vertices.end(), face.vertices.begin(), face.vertices.end());
				}
				boundingBox.setFromPoints(vertices);
			}

			void computeBoundingSphere(){
				if (boundingSphere == NULL){
					boundingSphere = new Sphere<T>();
				}
				std::vector<Vector3<T>> vertices;
				vertices.reserve(faces.size() * 3);
				for (Face3<T>& face : faces){
					vertices.insert(vertices.end(), face.vertices.begin(), face.vertices.end());
				}
				boundingSphere.setFromPoints(vertices);
			}

			void merge(const Geometry& geometry, const Matrix4<T>& matrix, int materialIndexOffset); //TODO

			void mergeMesh(const Mesh<T>& mesh){
				//Shorthand if?
				mesh.matrixAutoUpdate && mesh.updateMatrix();

				merge( mesh.geometry, mesh.matrix );

			}

			static inline unsigned materialIndexSort(const Face3<T>& a, const Face3<T>& b){
				return a.materialIndex - b.materialIndex;
			}

			void sortFacesByMaterialIndex(){

				std::sort(faces.begin(), faces.end(), materialIndexSort);

			}

			Geometry(const Geometry& geometry) : name(name), faces(faces) {}
			Geometry(Geometry&& geometry) : name(name), faces(faces) {}

			Geometry& copy(const Geometry& geometry){

				name = geometry.name;

				faces = geometry.faces;

				return *this;

			}

			Geometry& move(Geometry&& geometry){

				name = std::move(geometry.name);

				faces = std::move(geometry.faces);

				return *this;

			}

			Geometry& operator=(const Geometry& geometry){
				return copy(geometry);
			}

			Geometry& operator=(Geometry&& geometry) {
				return move(geometry);
			}

		};

#define GeometryDefined

	}

}

#endif
