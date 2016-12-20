#ifndef MYUPLAY_MYENGINE_MESH
#define MYUPLAY_MYENGINE_MESH

#include <string>
#include <cassert>

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

	std::shared_ptr<IGeometry<T>> geometry;
	std::shared_ptr<IMaterial> material;

	//void raycast(const Raycaster<T>&, std::vector<Intersection<T> >&); //TODO

	Mesh(IGeometry<T>* geometry, IMaterial* material)
	: Object3D<T>(Object3D<T>::ObjectType::MESH), geometry(geometry), material(material) {
		assert(geometry != nullptr);
		assert(material != nullptr);
	}

	Mesh(std::shared_ptr<IGeometry<T>> geo, std::shared_ptr<IMaterial> mat)
	: Object3D<T>(Object3D<T>::ObjectType::MESH), geometry(geo), material(mat) {
		assert(geo != nullptr);
		assert(mat != nullptr);
	}

	//Deep copy
	Mesh(const Mesh& m) : Object3D<T>(m) {
		*geometry = *(m.geometry);
		*material = *(m.material);
	}

	Mesh(Mesh&& m) : Object3D<T>(m), geometry(std::move(m.geometry)), material(std::move(m.material)) {}

	std::shared_ptr<IGeometry<T>> getGeometry() {
		return geometry;
	}

	std::shared_ptr<IMaterial> getMaterial() {
		return material;
	}

};

#ifdef NBINDING_MODE

namespace {

	using namespace MyUPlay::MyEngine;

	NBIND_CLASS(Mesh<>, Mesh) {

		inherit(Object3D<float>);

		construct<std::shared_ptr<IGeometry<float>>, std::shared_ptr<IMaterial>>();
		construct<const Mesh<>&>();

		getter(getGeometry);
		getter(getMaterial);

	}

}

#endif

#endif
