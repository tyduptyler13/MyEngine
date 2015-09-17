
#include <vector>

#include "Box3.hpp"

using namespace MyUPlay::MyEngine;
using namespace std;

template <typename T>
Box3<T>& Box3<T>::setFromObject(Object3D& object) {

	object.updateMatrixWorld(true);

	makeEmpty();

	object.traverse([this](const Object3D& node) {

		//This function assumes we don't have BufferGeometry. Everything is already native.
		for (Vector3<T> vertex : node.geometry.verticies) {

			vertex.applyMatrix4( node.matrixWorld );

			expandByPoint( vertex );

		}

	});

	return *this;

}

template <typename T>
Box3<T>& Box3<T>::applyMatrix4(const Matrix4& matrix) {

	vector<Vector3<T> > points(8);

	points[ 0 ].set( min.x, min.y, min.z ).applyMatrix4( matrix ); // 000
	points[ 1 ].set( min.x, min.y, max.z ).applyMatrix4( matrix ); // 001
	points[ 2 ].set( min.x, max.y, min.z ).applyMatrix4( matrix ); // 010
	points[ 3 ].set( min.x, max.y, max.z ).applyMatrix4( matrix ); // 011
	points[ 4 ].set( max.x, min.y, min.z ).applyMatrix4( matrix ); // 100
	points[ 5 ].set( max.x, min.y, max.z ).applyMatrix4( matrix ); // 101
	points[ 6 ].set( max.x, max.y, min.z ).applyMatrix4( matrix ); // 110
	points[ 7 ].set( max.x, max.y, max.z ).applyMatrix4( matrix );  // 111

	makeEmpty();
	setFromPoints( points );

	return *this;

}

