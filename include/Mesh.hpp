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
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Triangle.hpp"

template <typename T = float>
struct MyUPlay::MyEngine::Mesh : public Object3D<T> {

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

	virtual ~Mesh(){}

	std::shared_ptr<IGeometry<T>> getGeometry() {
		return geometry;
	}

	std::shared_ptr<IMaterial> getMaterial() {
		return material;
	}

	/*
	virtual void raycast(const Raycaster<T>& r, std::vector<Intersection<T> >& intersections) const {

		Matrix4<T> inverseMatrix;
		Ray<T> ray;
		Sphere<T> sphere;

		Vector3<T> tmpA, tmpB, tmpC;

		Vector2<T> uvA, uvB, uvC;

		Vector3<T> barycoord;

		Vector3<T> intersectionPoint, intersectionPointWorld;

		const auto uvIntersection = [&](const Vector3<T>& point, const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3,
				Vector2<T> uv1, Vector2<T> uv2, Vector2<T> uv3)->Vector2<T> {
			Triangle<T>::barycoordFromPoint(point, p1, p2, p3, barycoord);

			uv1.multiplyScalar(barycoord.x);
			uv2.multiplyScalar(barycoord.y);
			uv3.multiplyScalar(barycoord.z);

			uv1.add(uv2).add(uv3);

			return uv1;

		};

		struct CheckResult {
			T distance;
			Vector3<T> point;
			const Mesh* object;
		};

		const auto checkIntersection = [&](const Mesh* obj, const Raycaster<T>& raycaster, const Ray<T>& ray,
				const Vector3<T>& pA, const Vector3<T>& pB, const Vector3<T>& pC, const Vector3<T>& point)-> std::unique_ptr<CheckResult> {

			std::shared_ptr<Vector3<T>> intersect;
			std::shared_ptr<IMaterial>& mat = obj->material;

			if (mat->side == BackSide){
				intersect = ray.intersectTriangle(pC, pB, pA, true, point);
			} else {
				intersect = ray.intersectTriangle(pA, pB, pC, mat->side != DoubleSide, point);
			}

			if (intersect == nullptr) return nullptr;

			intersectionPointWorld.copy(point);
			intersectionPointWorld.applyMatrix4(obj->matrixWorld);

			T distance = raycaster.ray.origin.distanceTo(intersectionPointWorld);

			if (distance < raycaster.near || distance > raycaster.far) return nullptr;

			auto ret = std::make_unique<CheckResult>();
			ret->distance = distance;
			ret->point.copy(intersectionPointWorld);
			ret->object = obj;

			return ret;

		};

		//TODO

		//Both material and geometry must be properly set.
		if (material == nullptr || geometry == nullptr) return;

		if (geometry->boundingSphere == nullptr) geometry->computeBoundingSphere();

		sphere.copy(geometry->boundingSphere);
		sphere.applyMatrix4(matrixWorld);

		if (r.ray.intersectsSphere(sphere) == false) return;

		inverseMatrix.getInverse(matrixWorld);
		ray.copy(r.ray).applyMatrix4(inverseMatrix);

		if (geometry->boundingBox != nullptr) {
			if (ray.intersectsBox(*geometry->boundingBox == false)) return;
		}

		//TODO need to find polymorphic way to do the intersection

	}
	*/


};

#endif
