#include "Geometry.hpp"

using namespace MyUPlay::MyEngine;
using namespace std;


template <typename T>
void Geometry<T>::applyMatrix(const  Matrix4<T>& matrix){
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

template <typename T>
Geometry<T>& Geometry<T>::normalize(){
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

template <typename T>
void Geometry<T>::computeFaceNormals(){
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

template <typename T>
void Geometry<T>::computeVertexNormals(bool areaWeighted){
    Vector3<T> v, vl, f, fl, face;

    std::vector<Vector3<T> > vertices2(vertices.length );

//    ??????? does do anything? does it matter? does life.....? lulz
//    for ( v = 0, vl = vertices.length; v < vl; v ++ ) {
//
//        vertices2[ v ] = Vector3<T>();
//
//    }

    if ( areaWeighted ) {

        // vertex normals weighted by triangle areas
        // http://www.iquilezles.org/www/articles/normals/normals.htm

        Vector3<T> vA, vB, vC;
        Vector3<T> cb;
        Vector3<T> ab;

        for ( f = 0, fl = faces.length; f < fl; f ++ ) {

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

        for ( f = 0, fl = faces.length; f < fl; f ++ ) {

            Face3<T> face = faces[ f ];

            vertices2[ face.a ].add( face.normal );
            vertices2[ face.b ].add( face.normal );
            vertices2[ face.c ].add( face.normal );

        }

    }

    for ( v = 0, vl = vertices.length; v < vl; v ++ ) {

        vertices2[ v ].normalize();

    }

    for ( f = 0, fl = faces.length; f < fl; f ++ ) {

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

template <typename  T>
void Geometry<T>::computeMorphNormals(){

    Vector3<T> i, il, f, fl, face;

    // save original normals
    // - create temp variables on first access
    //   otherwise just copy (for faster repeated calls)

    for (f = 0, fl = faces.length; f < fl; f ++ ) {

        face = faces[ f ];

        if ( ! face.__originalFaceNormal ) {

            face.__originalFaceNormal = face.normal.clone();

        } else {

            face.__originalFaceNormal.copy( face.normal );

        }

        if ( ! face.__originalVertexNormals ) face.__originalVertexNormals = std::vector<Face3<T> >();

        for ( i = 0, il = face.vertexNormals.length; i < il; i ++ ) {

            if ( ! face.__originalVertexNormals[ i ] ) {

                face.__originalVertexNormals[ i ] = face.vertexNormals[ i ].clone();

            } else {

                face.__originalVertexNormals[ i ].copy( face.vertexNormals[ i ] );

            }

        }

    }

    // use temp geometry to compute face and vertex normals for each morph

    Vector3<T>  tmpGeo;
    tmpGeo.faces = faces;

    for ( i = 0, il = morphTargets.length; i < il; i ++ ) {

        // create on first access

        if ( ! morphNormals[ i ] ) {

            morphNormals[ i ] = {};
            morphNormals[ i ].faceNormals = [];
            morphNormals[ i ].vertexNormals = [];

            var dstNormalsFace = morphNormals[ i ].faceNormals;
            var dstNormalsVertex = morphNormals[ i ].vertexNormals;

            var faceNormal, vertexNormals;

            for ( f = 0, fl = faces.length; f < fl; f ++ ) {

                //TODO  now  lulz  why so slow and why can I not do it
                Vector3<T>  faceNormal;
                faceNormal = new THREE.Vector3();
                vertexNormals = { a: new THREE.Vector3(), b: new THREE.Vector3(), c: new THREE.Vector3() };

                dstNormalsFace.push( faceNormal );
                dstNormalsVertex.push( vertexNormals );

            }

        }

        Vector3<T> morphNormals = morphNormals[ i ];

        // set vertices to morph target

        tmpGeo.vertices = morphTargets[ i ].vertices;

        // compute morph normals

        tmpGeo.computeFaceNormals();
        tmpGeo.computeVertexNormals();

        // store morph normals

        Vector3<T> faceNormal, vertexNormals;

        for ( f = 0, fl = faces.length; f < fl; f ++ ) {

            face = faces[ f ];

            faceNormal = morphNormals.faceNormals[ f ];
            vertexNormals = morphNormals.vertexNormals[ f ];

            faceNormal.copy( face.normal );

            vertexNormals.a.copy( face.vertexNormals[ 0 ] );
            vertexNormals.b.copy( face.vertexNormals[ 1 ] );
            vertexNormals.c.copy( face.vertexNormals[ 2 ] );

        }

    }

    // restore original normals

    for ( f = 0, fl = faces.length; f < fl; f ++ ) {

        face = faces[ f ];

        face.normal = face.__originalFaceNormal;
        face.vertexNormals = face.__originalVertexNormals;

        }

}


template <typename T>
void Geometry<T>::mergeMesh(const Mesh<T>& mesh){
    mesh.matrixAutoUpdate && mesh.updateMatrix();

    merge( mesh.geometry, mesh.matrix );

}

template <typename T>
unsigned Geometry<T>::mergeVertices(){
    var verticesMap = {}; // Hashmap for looking up vertices by position coordinates (and making sure they are unique)
    std::vector<Vector3<T> > unique, changes;

    Vector3<T> v;
    T key;
    T precisionPoints = 4; // number of decimal points, e.g. 4 for epsilon of 0.0001
    T precision = pow( 10, precisionPoints );
    Vector3<T> i, il, face;
    std::vector<unsigned> indices;
    unsigned j, jl;

    for ( i = 0, il = vertices.length; i < il; i ++ ) {

        v = vertices[ i ];
        key = round( v.x * precision ) + '_' + round( v.y * precision ) + '_' + round( v.z * precision );

        //TODO
        if ( verticesMap[ key ] == undefined ) {

            verticesMap[ key ] = i;
            unique.push( vertices[ i ] );
            changes[ i ] = unique.length - 1;

        } else {

            //console.log('Duplicate vertex found. ', i, ' could be using ', verticesMap[key]);
            changes[ i ] = changes[ verticesMap[ key ] ];

        }

    }


    // if faces are completely degenerate after merging vertices, we
    // have to remove them from the geometry.
    std::vector<unsigned> faceIndicesToRemove;

    for ( i = 0, il = faces.length; i < il; i ++ ) {

        face = faces[ i ];

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

    for ( i = faceIndicesToRemove.size() - 1; i >= 0; i -- ) {

        unsigned idx = faceIndicesToRemove[ i ];

        faces.splice( idx, 1 );

        for ( j = 0, jl = faceVertexUvs.length; j < jl; j ++ ) {

            faceVertexUvs[ j ].splice( idx, 1 );

        }

    }

    // Use unique set of vertices

    unsigned diff = vertices.length - unique.length;
    vertices = unique;
    return diff;

}

template <typename T>
unsigned materialIndexSort(const Face3<T>& a, const Face3<T>& b){

        return a.materialIndex - b.materialIndex;
}


template <typename T>
void Geometry<T>::sortFacesByMaterialIndex(){

    // tag faces

    for ( unsigned i = 0; i < faces.size(); i ++ ) {

        faces[ i ]._id = i;

    }

    // sort faces
    faces.sort( materialIndexSort );

    // sort uvs
    // TODO



    std::vector<Vector2<T> > uvs1 = faceVertexUvs[ 0 ];
    std::vector<Vector2<T> > uvs2 = faceVertexUvs[ 1 ];

    std::vector<Vector2<T> > newUvs1, newUvs2;

    if ( uvs1 && uvs1.length == faces.size() ){ newUvs1.empty();}
    if ( uvs2 && uvs2.length == faces.size() ){ newUvs2.empty();}


    for ( unsigned i = 0; i < faces.size(); i ++ ) {
        //TOD
        unsigned id = faces[ i ]._id;

        if ( newUvs1 ) newUvs1.push( uvs1[ id ] );
        if ( newUvs2 ) newUvs2.push( uvs2[ id ] );

    }

    if ( newUvs1 ) faceVertexUvs[ 0 ] = newUvs1;
    if ( newUvs2 ) faceVertexUvs[ 1 ] = newUvs2;

}


template <typename T>
Geometry<T>& Geometry<T>::copy(const Geometry<T>& geometry){
    vertices.empty();
    faces.empty();
    faceVertexUvs.empty();

    Vector3<T> vertices = geometry.vertices;

    for ( unsigned i = 0, il = vertices.length; i < il; i ++ ) {

        vertices.push( vertices[ i ].clone() );

    }

    Vector3<T> faces = geometry.faces;

    for ( unsigned i = 0, il = faces.length; i < il; i ++ ) {

        faces.push( faces[ i ].clone() );

    }

    for ( unsigned i = 0, il = geometry.faceVertexUvs.length; i < il; i ++ ) {

        std::vector<Vector2<T> > faceVertexUvs = geometry.faceVertexUvs[ i ];

        //TODO
        if ( faceVertexUvs[ i ] == undefined ) {

            faceVertexUvs[ i ].empty();

        }

        for ( unsigned j = 0, jl = faceVertexUvs.length; j < jl; j ++ ) {

            Vector2<T> uvs = faceVertexUvs[ j ], uvsCopy;

            for ( unsigned k = 0, kl = uvs.length; k < kl; k ++ ) {

                Vector2<T> uv = uvs[ k ];

                uvsCopy.push( uv.clone() );

            }

            faceVertexUvs[ i ].push( uvsCopy );

        }

    }
    return *this;

}
