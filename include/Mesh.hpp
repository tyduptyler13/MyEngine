#ifndef MYUPLAY_MYENGINE_MESH
#define MYUPLAY_MYENGINE_MESH

#include <string>

namespace MyUPlay {
	namespace MyEngine {
		template <typename> struct Mesh;
	}
}

#include "Object3D.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Raycaster.hpp"

template <typename T = float>
struct MyUPlay::MyEngine::Mesh : public Object3D<T> {

	std::unique_ptr<IGeometry<T>> geometry;
	std::unique_ptr<IMaterial> material;

	//void raycast(const Raycaster<T>&, std::vector<Intersection<T> >&); //TODO

	Mesh(IGeometry<T>* geometry, IMaterial* material)
	: Object3D<T>(Object3D<T>::ObjectType::MESH), geometry(geometry), material(material) {}

	Mesh(const Mesh& m){
		*geometry = *(m.geometry);
		*material = *(m.material);
	}

	Mesh(Mesh&& m) : geometry(std::move(m.geometry)), material(std::move(m.material)) {}

};

#endif
