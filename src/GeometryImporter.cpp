#include "GeometryImporter.hpp"
#include "Mesh.hpp"
#include "BufferGeometry.hpp"
#include "GLES2MaterialLib.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <thread>
#include <memory>

#include <cassert>

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace Assimp;

Object3D<float>* GeometryImporter::ImportAsset(string s) {
	Importer importer;

	const aiScene* scene = importer.ReadFile(s,
			aiProcess_Triangulate |
			aiProcess_GenNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_OptimizeMeshes |
			aiProcess_OptimizeGraph |
			aiProcess_GenUVCoords );

	if ( !scene ) return nullptr;

	Object3D<float>* obj = new Object3D<float>();
	obj->name = "Import Group";

	//TODO Lights

	//TODO Materials

	//TODO Meshes
	for (unsigned i = 0; i < scene->mNumMeshes; ++i){
		shared_ptr<BufferGeometry<float>> geo = make_shared<BufferGeometry<float>>();
		shared_ptr<IMaterial> mat = GLES2CreateNormalMaterial();
		assert(geo != nullptr);
		assert(mat != nullptr);
		Mesh<float>* mesh = new Mesh<float>(geo, mat); //TODO Replace this material with correct material above when finished

		mesh->name = scene->mMeshes[i]->mName.C_Str();

		unsigned num = scene->mMeshes[i]->mNumVertices;

		geo->vertices.reserve(num * 3);
		geo->normals.reserve(num * 3);
		//geo->uvs.reserve(num * 2); FIXME //UVs are broken for now

		for (unsigned v = 0; v < scene->mMeshes[i]->mNumVertices; ++v) {
			const aiVector3D& vert = scene->mMeshes[i]->mVertices[v];
			const aiVector3D& norm = scene->mMeshes[i]->mNormals[v];
			//const aiVector3D& uv = scene->mMeshes[i]->mTextureCoords[2][v]; FIXME
			geo->vertices.insert(geo->vertices.end(), { vert.x, vert.y, vert.z });
			geo->normals.insert(geo->normals.end(), { norm.x, norm.y, norm.z });
			//geo->uvs.insert(geo->uvs.end(), { uv.x, uv.y } ); FIXME
		}

		geo->indices.reserve(scene->mMeshes[i]->mNumFaces);

		for (unsigned f = 0; f < scene->mMeshes[i]->mNumFaces; ++f) {
			const aiFace& face = scene->mMeshes[i]->mFaces[f];
			geo->indices.insert(geo->indices.end(), { face.mIndices[0], face.mIndices[1], face.mIndices[2] } );
		}

		//TODO Bones

		geo->indicesNeedUpdate = true;

		obj->add(mesh);

	}

	//TODO Cameras

	//TODO Textures

	//TODO Animations

	return obj;

}
