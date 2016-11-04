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
#include "Mesh.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct Mesh;

		/**
		 * Geometry Interface, for pointers that don't care about the underlying geometry.
		 */
		template <typename T>
		struct IGeometry {

			Math::UUID uuid = Math::generateUUID();

			std::string name;

			std::unique_ptr<Box3<T> > boundingBox;
			std::unique_ptr<Sphere<T> > boundingSphere;

			bool verticesNeedUpdate = false,
					normalsNeedUpdate = false,
					colorsNeedUpdate = false;

			virtual void applyMatrix(const Matrix4<T>& matrix) = 0;

			virtual void computeBoundingBox() = 0;
			virtual void computeBoundingSphere() = 0;

			virtual bool hasIndexedVertices(){
				return false;
			}

			virtual bool hasIndexedNormals(){
				return false;
			}

			virtual std::vector<Vector3<T>> getVertices() = 0;
			virtual std::vector<unsigned> getVertexIndices() {
				return {}; //Empty list;
			}

			virtual std::vector<Vector3<T>> getNormals() = 0;
			virtual std::vector<unsigned> getNormalIndices() {
				return {};
			}

			virtual std::vector<Color> getColors() = 0;

			/**
			 * Returns the number elements in the Geometry.
			 *
			 * An element is a single set of Vertex, Normal, and Color.
			 *
			 * To get faces divide this by 3. (All faces are triangles)
			 */
			virtual unsigned size() = 0;

			IGeometry& operator=(const IGeometry& geometry) {
				name = geometry.name;
				return *this;
			}

			IGeometry& operator=(IGeometry&& geometry) {
				name = std::move(geometry.name);
				return *this;
			}

			virtual ~IGeometry(){}

			IGeometry(){}
			IGeometry(std::string name) : name(name) {}

			template <typename T2>
			IGeometry(const IGeometry<T2>& g) : name(g.name) {}

			template <typename T2>
			IGeometry(IGeometry<T2>&& g) : name(std::move(g.name)) {}
		};

		/**
		 * Abstract geometry
		 *
		 * Contains classes that get specialized methods specifically for the
		 * derived classes.
		 */
		template <typename T, class Derived>
		struct AGeometry : public IGeometry<T> {

			Derived& rotateX(T angle) {
				Matrix4<T> m1;
				m1.makeRotationX(angle);
				applyMatrix(m1);
				return *this;
			}

			Derived& rotateY(T angle) {
				Matrix4<T> m1;
				m1.makeRotationY(angle);
				applyMatrix(m1);
				return *this;
			}

			Derived& rotateZ(T angle) {
				Matrix4<T> m1;
				m1.makeRotationZ(angle);
				applyMatrix(angle);
				return *this;
			}

			Derived& translate(T x, T y, T z) {
				Matrix4<T> m1;
				m1.makeTranslation(x, y, z);
				applyMatrix(m1);
				return *this;
			}

			Derived& scale(T x, T y, T z) {
				Matrix4<T> m1;
				m1.makeScale(x, y, z);
				applyMatrix(m1);
				return *this;
			}

			Derived& lookAt(const Vector3<T>& v){
				Matrix4<T> m1;
				m1.lookAt(v);
				applyMatrix(m1);
				return *this;
			}

			Vector3<T> center(){
				this->computeBoundingBox();
				Vector3<T> offset = this->boundingBox->center().negate();
				translate(offset.x, offset.y, offset.z);
				return offset;
			}

			Derived& normalize(){
				this->computeBoundingSphere();

				Vector3<T> center = this->boundingSphere->center;
				T radius = this->boundingSphere->radius;

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

		};

		/**
		 * This is the simplest geometry. All faces are explicitly present and modifications are
		 * as simple as locating a face and modifying its values. A counter example would be the buffer
		 * geometry, which all faces have indexed values and thus requires extra work to modify a value.
		 */
		template <typename T>
		struct SimpleGeometry : public AGeometry<T, SimpleGeometry<T>> {

			std::vector<Face3<T> > faces;

			SimpleGeometry(const SimpleGeometry& geometry) : IGeometry<T>(geometry), faces(geometry.faces) {}
			SimpleGeometry(SimpleGeometry&& geometry) : IGeometry<T>(geometry), faces(geometry.faces) {}

			void applyMatrix(const  Matrix4<T>& matrix) override {
				Matrix3<T> normalMatrix = Matrix3<T>().getNormalMatrix( matrix );

				for (Face3<T>& f : faces){

					for (Vector3<T>& v : f.vertices){
						v = v.applyMatrix4(matrix);
					}

					for (Vector3<T>& n : f.normals){
						n = n.applyMatrix3(normalMatrix).normalize();
					}

				}

				if ( this->boundingBox != NULL ) {

					computeBoundingBox();

				}

				if ( this->boundingSphere != NULL ) {

					computeBoundingSphere();

				}

				this->verticesNeedUpdate = true;
				this->normalsNeedUpdate = true;

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
						tmp.insert(tmp.end(), face.normals.begin(), face.normals.end());
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


			void computeBoundingBox() override {
				if (this->boundingBox == nullptr){
					this->boundingBox = new Box3<T>();
				}
				std::vector<Vector3<T>> vertices;
				vertices.reserve(faces.size() * 3);
				for (Face3<T>& face : faces){
					vertices.insert(vertices.end(), face.vertices.begin(), face.vertices.end());
				}
				this->boundingBox.setFromPoints(vertices);
			}

			void computeBoundingSphere() override {
				if (this->boundingSphere == nullptr){
					this->boundingSphere = new Sphere<T>();
				}
				std::vector<Vector3<T>> vertices;
				vertices.reserve(faces.size() * 3);
				for (Face3<T>& face : faces){
					vertices.insert(vertices.end(), face.vertices.begin(), face.vertices.end());
				}
				this->boundingSphere.setFromPoints(vertices);
			}

			void merge(const SimpleGeometry& geometry, const Matrix4<T>& matrix, int materialIndexOffset); //TODO

			void mergeMesh(std::shared_ptr<Mesh<T>> mesh){
				//Shorthand if?
				mesh->matrixAutoUpdate && mesh->updateMatrix();

				merge( mesh->geometry, mesh->matrix );

			}

			static inline unsigned materialIndexSort(const Face3<T>& a, const Face3<T>& b){
				return a.materialIndex - b.materialIndex;
			}

			void sortFacesByMaterialIndex(){

				std::sort(faces.begin(), faces.end(), materialIndexSort);

			}

			SimpleGeometry& copy(const SimpleGeometry& geometry){

				IGeometry<T>::operator=(geometry);

				faces = geometry.faces;

				return *this;

			}

			SimpleGeometry& move(SimpleGeometry&& geometry){

				IGeometry<T>::operator=(geometry);

				faces = std::move(geometry.faces);

				return *this;

			}

			SimpleGeometry& operator=(const SimpleGeometry& geometry){
				return copy(geometry);
			}

			SimpleGeometry& operator=(SimpleGeometry&& geometry) {
				return move(geometry);
			}

		};

	}

}

#endif
