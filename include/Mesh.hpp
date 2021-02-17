#pragma once

#include <string>
#include <cassert>


namespace MyEngine {
	template<typename>
	struct Mesh;
}


#include "Object3D.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Raycaster.hpp"

template<typename T = float>
struct MyEngine::Mesh : public Object3D<T> {

	std::shared_ptr<IGeometry<T>> geometry;
	std::shared_ptr<IMaterial> material;

	Mesh(IGeometry<T>* geometry, IMaterial* material)
			: Object3D<T>(Object3D<T>::ObjectType::MESH), geometry(geometry), material(material) {}

	Mesh(std::shared_ptr<IGeometry<T>> geo, std::shared_ptr<IMaterial> mat)
			: Object3D<T>(Object3D<T>::ObjectType::MESH), geometry(geo), material(mat) {}

	//Shallow copy
	Mesh(const Mesh& m) : Object3D<T>(m) {
		geometry = m.geometry;
		material = m.material;
	}

	Mesh(Mesh&& m) : Object3D<T>(m), geometry(std::move(m.geometry)), material(std::move(m.material)) {}

	virtual ~Mesh() {}

	std::shared_ptr<IGeometry<T>> getGeometry() {
		return geometry;
	}

	std::shared_ptr<IMaterial> getMaterial() {
		return material;
	}

	virtual void raycast(std::shared_ptr<Object3D<T>>& obj, const Raycaster<T>& r,
	                     std::vector<Intersection<T> >& intersections) const {

		//Both material and geometry must be properly set.
		if (material == nullptr || geometry == nullptr) return;

		if (geometry->boundingSphere == nullptr) {
			geometry->computeBoundingSphere();
		}

		Matrix4<T> inverseMatrix;

		inverseMatrix.getInverse(this->matrixWorld);

		Ray<T> ray = r.ray;
		ray.applyMatrix4(inverseMatrix);

		if (!ray.intersectSphere(*geometry->boundingSphere)) return;

		if (geometry->boundingBox != nullptr) {
			if (!ray.intersectBox(*geometry->boundingBox)) return;
		}

		geometry->raycast(obj, r, intersections, material->side);

	}

};

