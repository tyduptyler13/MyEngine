#ifndef MYUPLAY_MYENGINE_GEOMETRY
#define MYUPLAY_MYENGINE_GEOMETRY

#include <vector>
#include <string>
#include <memory>
#include <string>
#include <unordered_map>
#include <array>

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
		struct FaceNormal {
			Vector3<T> a, b, c;
		};

		template <typename T>
		struct MorphTargets {
			std::string name;
			std::vector<Vector3<T> > vertices;
		};

		template <typename T>
		struct MorphNormals {
			std::string name;
			std::vector<Vector3<T> > faceNormals;
			std::vector<FaceNormal<T> > vertexNormals;
		};

		template <typename T>
		class Geometry {

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

			void applyMatrix(const  Matrix4<T>& matrix){
				Matrix3<T> normalMatrix = Matrix3<T>().getNormalMatrix( matrix );

				for (unsigned i = 0, il = vertices.size(); i < il; i ++ ) {

					vertices[i].applyMatrix4(matrix);

				}

				for (unsigned i = 0, il = faces.size(); i < il; i ++ ) {

					Face3<T>& face = faces[ i ];
					face.normal.applyMatrix3(normalMatrix).normalize();

					for(unsigned j = 0, jl = face.vertexNormals.size(); j < jl; j ++ ) {

						face.vertexNormals[ j ].applyMatrix3( normalMatrix ).normalize();

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

			Geometry& fromBufferGeometry(const BufferGeometry<T>& geometry);

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

				return;
			}

			void computeFaceNormals(){
				Vector3<T> cb;
				Vector3<T> ab;

				for ( unsigned f = 0, fl = faces.length; f < fl; f ++ ) {

					Face3<T> face = faces[ f ];

					Vector3<T> vA = vertices[ face.a ];
					Vector3<T> vB = vertices[ face.b ];
					Vector3<T> vC = vertices[ face.c ];

					cb.subVectors( vC, vB );
					ab.subVectors( vA, vB );
					cb.cross( ab );

					cb.normalize();

					face.normal.copy( cb );

				}
			}
			void computeVertexNormals(bool areaWeighted){

				std::vector<Vector3<T> > vertices2(vertices.length );

				if ( areaWeighted ) {

					// vertex normals weighted by triangle areas
					// http://www.iquilezles.org/www/articles/normals/normals.htm

					Vector3<T> vA, vB, vC;
					Vector3<T> cb;
					Vector3<T> ab;

					for (unsigned f = 0, fl = faces.length; f < fl; f ++ ) {

						Face3<T> face = faces[ f ];

						vA = vertices[ face.a ];
						vB = vertices[ face.b ];
						vC = vertices[ face.c ];

						cb.subVectors( vC, vB );
						ab.subVectors( vA, vB );
						cb.cross( ab );

						vertices2[ face.a ].add( cb );
						vertices2[ face.b ].add( cb );
						vertices2[ face.c ].add( cb );

					}

				} else {

					for (unsigned f = 0, fl = faces.length; f < fl; f ++ ) {

						Face3<T> face = faces[ f ];

						vertices2[ face.a ].add( face.normal );
						vertices2[ face.b ].add( face.normal );
						vertices2[ face.c ].add( face.normal );

					}

				}

				for (unsigned v = 0, vl = vertices.length; v < vl; v ++ ) {

					vertices2[ v ].normalize();

				}

				for (unsigned f = 0, fl = faces.length; f < fl; f ++ ) {

					Face3<T> face = faces[ f ];

					Face3<T> vertexNormals = face.vertexNormals;

					if ( vertexNormals.length == 3 ) {

						vertexNormals[ 0 ].copy( vertices[ face.a ] );
						vertexNormals[ 1 ].copy( vertices[ face.b ] );
						vertexNormals[ 2 ].copy( vertices[ face.c ] );

					} else {

						vertexNormals[ 0 ] = vertices[ face.a ].clone();
						vertexNormals[ 1 ] = vertices[ face.b ].clone();
						vertexNormals[ 2 ] = vertices[ face.c ].clone();

					}

				}

			}
			void computeMorphNormals(){
				// save original normals
				// - create temp variables on first access
				//   otherwise just copy (for faster repeated calls)

				for (unsigned f = 0, fl = faces.length; f < fl; f ++ ) {

					Face3<T>& face = faces[ f ];

					if ( ! face.__originalFaceNormal ) {

						face.__originalFaceNormal = face.normal.clone();

					} else {

						face.__originalFaceNormal.copy( face.normal );

					}

					if ( ! face.__originalVertexNormals ) face.__originalVertexNormals = std::vector<Face3<T> >();

					for (unsigned i = 0, il = face.vertexNormals.length; i < il; i ++ ) {

						if ( ! face.__originalVertexNormals[ i ] ) {

							face.__originalVertexNormals[ i ] = face.vertexNormals[ i ].clone();

						} else {

							face.__originalVertexNormals[ i ].copy( face.vertexNormals[ i ] );

						}

					}

				}

				// use temp geometry to compute face and vertex normals for each morph
				Geometry<T> tmpGeo;
				tmpGeo.faces = faces;

				morphNormals.resize(morphTargets.size());

				for (unsigned i = 0, il = morphTargets.length; i < il; i ++ ) {

					MorphNormals<T>& morphNormals = this->morphNormals[ i ];

					// set vertices to morph target

					tmpGeo.vertices = morphTargets[ i ].vertices;

					// compute morph normals

					tmpGeo.computeFaceNormals();
					tmpGeo.computeVertexNormals();

					for (unsigned f = 0, fl = faces.length; f < fl; f ++ ) {

						Face3<T>& face = faces[ f ];

						FaceNormal<T>& faceNormal = morphNormals.faceNormals[ f ];
						std::vector<FaceNormal<T> >& vertexNormals = morphNormals.vertexNormals[ f ];

						faceNormal = face.normal;

						vertexNormals.a = face.vertexNormals[ 0 ];
						vertexNormals.b = face.vertexNormals[ 1 ];
						vertexNormals.c = face.vertexNormals[ 2 ];

					}

				}

				// restore original normals

				for (unsigned f = 0, fl = faces.length; f < fl; f ++ ) {

					Face3<T>& face = faces[ f ];

					face.normal = face.__originalFaceNormal;
					face.vertexNormals = face.__originalVertexNormals;

				}

			}

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

			void mergeMesh(const Mesh<T>& mesh){
				//Shorthand if?
				mesh.matrixAutoUpdate && mesh.updateMatrix();

				merge( mesh.geometry, mesh.matrix );

			}

			unsigned mergeVertices(){
				std::unordered_map<std::string, unsigned> verticesMap; // Hashmap for looking up vertices by position coordinates (and making sure they are unique)
				std::vector<Vector3<T> > unique, changes(vertices.size());

				T precisionPoints = 4; // number of decimal points, e.g. 4 for epsilon of 0.0001
				T precision = pow( 10, precisionPoints );
				std::array<unsigned, 3> indices;

				for (unsigned i = 0, il = vertices.size(); i < il; i ++ ) {

					Vector3<T>& v = vertices[ i ];
					std::string key = round( v.x * precision ) + "_" + round( v.y * precision ) + "_" + round( v.z * precision );

					if ( verticesMap.find(key) == verticesMap.end() ) {

						verticesMap[key] = i;
						unique.push_back( vertices[ i ] );
						changes[ i ] = unique.size() - 1;

					} else {

						//console.log('Duplicate vertex found. ', i, ' could be using ', verticesMap[key]);
						changes[ i ] = changes[ verticesMap[ key ] ];

					}

				}


				// if faces are completely degenerate after merging vertices, we
				// have to remove them from the geometry.
				std::vector<unsigned> faceIndicesToRemove;

				for (unsigned i = 0, il = faces.length; i < il; i ++ ) {

					Face3<T>& face = faces[ i ];

					face.a = changes[ face.a ];
					face.b = changes[ face.b ];
					face.c = changes[ face.c ];

					indices = { face.a, face.b, face.c };

					unsigned dupIndex = - 1;

					// if any duplicate vertices are found in a Face3
					// we have to remove the face as nothing can be saved
					for ( unsigned n = 0; n < 3; n ++ ) {

						if ( indices[ n ] == indices[ ( n + 1 ) % 3 ] ) {

							dupIndex = n;
							faceIndicesToRemove.push_back( i );
							break;

						}

					}

				}

				//FIXME This could be significantly improved (For every removed face, the run time is the length in faces)
				for (int i = faceIndicesToRemove.size() - 1; i >= 0; i -- ) {

					unsigned idx = faceIndicesToRemove[ i ];

					faces.erase( faces.begin() + idx );

					for (unsigned j = 0, jl = faceVertexUvs.length; j < jl; j ++ ) {

						faceVertexUvs[ j ].erase(faceVertexUvs.begin() + idx);

					}

				}

				// Use unique set of vertices

				unsigned diff = vertices.length - unique.length;
				vertices = unique;
				return diff;

			}
			unsigned materialIndexSort(const Face3<T>& a, const Face3<T>& b){
				return a.materialIndex - b.materialIndex;
			}
			void sortFacesByMaterialIndex(){

				// tag faces

				for ( unsigned i = 0; i < faces.size(); i ++ ) {
					//TODO
					faces[ i ]._id = i;

				}

				// sort faces
				faces.sort( materialIndexSort );

				// sort uvs

				std::vector<Vector2<T> >& uvs1 = faceVertexUvs[ 0 ];
				std::vector<Vector2<T> >& uvs2 = faceVertexUvs[ 1 ];

				std::vector<Vector2<T> > newUvs1, newUvs2;
				bool use1 = false, use2 = false;

				if ( uvs1 && uvs1.size() == faces.size() ){ use1 = true; }
				if ( uvs2 && uvs2.size() == faces.size() ){ use2 = true; }


				for ( unsigned i = 0; i < faces.size(); i ++ ) {
					//TODO
					unsigned id = faces[ i ]._id;

					if ( use1 ) newUvs1.push( uvs1[ id ] );
					if ( use2 ) newUvs2.push( uvs2[ id ] );

				}

				if ( use1 ) faceVertexUvs[ 0 ] = newUvs1;
				if ( use2 ) faceVertexUvs[ 1 ] = newUvs2;

			}

			Geometry(const Geometry& geometry){
				copy(geometry);
			}

			Geometry& copy(const Geometry& geometry){
				vertices = geometry.vertices;
				faces = geometry.faces;
				faceVertexUvs = geometry.faceVertexUvs;

				return *this;

			}
			Geometry& operator=(const Geometry& geometry){
				copy(geometry);
			}

		};

#define GeometryDefined

	}

}

#endif
