#ifndef MYUPLAY_MYENGINE_SIMPLEGEOMETRY
#define MYUPLAY_MYENGINE_SIMPLEGEOMETRY

//TODO

namespace MyUPlay {
	namespace MyEngine {

	}
}

/**
 * This is the simplest geometry. All faces are explicitly present and modifications are
 * as simple as locating a face and modifying its values. A counter example would be the buffer
 * geometry, which all faces have indexed values and thus requires extra work to modify a value.
 */
template <typename T>
struct SimpleGeometry : public IGeometry<T> {

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


	void computeBoundingBox() {
		if (!this->boundingBoxDirty) return;
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

	void computeBoundingSphere() {
		if (!this->boundingSphereDirty) return;
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

#endif
