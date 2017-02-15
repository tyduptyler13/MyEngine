#ifndef MYUPLAY_MYENGINE_RAYCASTER
#define MYUPLAY_MYENGINE_RAYCASTER

#include <limits>
#include <memory>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct Intersection;

		template <typename T>
		class Raycaster;

	}

}

#include "Ray.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "PerspectiveCamera.hpp"
#include "OrthographicCamera.hpp"
#include "Face3.hpp"
#include "Object3D.hpp"
#include "Triangle.hpp"
#include "Constants.hpp"

template <typename T>
struct MyUPlay::MyEngine::Intersection {
	T distance;
	Vector3<T> point;
	std::shared_ptr<Object3D<T>> object;
	Vector2<T> uv;
	Face3<T> face;
	unsigned faceIndex;
};

template <typename T>
class MyUPlay::MyEngine::Raycaster {

private:

	static T ascSort(Intersection<T>& a, Intersection<T>& b){
		return a.distance - b.distance;
	}

	static void intersectObject(const std::shared_ptr<Object3D<T>>& o, const Raycaster<T>& r, std::vector<Intersection<T>>& intersects, bool recursive){

		if (o->visible == false) return;

		o->raycast(r, intersects);

		if (recursive == true) {

			for (const std::shared_ptr<Object3D<T>>& child : o->children) {
				intersectObject(child, r, intersects, true);
			}

		}

	}

public:

	Ray<T> ray;
	T near, far;

	Raycaster(Vector3<T> origin, Vector3<T> direction, T near = 0, T far = std::numeric_limits<T>::max())
	: ray(origin, direction), near(near), far(far) {}

	void set(Vector3<T> origin, Vector3<T> direction) {
		ray.set(origin, direction);
	}

	void setFromCamera(Vector2<T> coords, const PerspectiveCamera<T>& camera) {

		ray.origin.setFromMatrixPosition(camera.matrixWorld);
		ray.direction.set(coords.x, coords.y, 0.5).unproject(camera).sub(ray.origin).normalize();

	}

	void setFromCamera(Vector2<T> coords, const OrthographicCamera<T>& camera){

		ray.origin.set(coords.x, coords.y, (camera.near + camera.far) / (camera.near - camera.far)).unproject(camera);
		ray.direction.set(0,0,-1).transformDirection(camera.matrixWorld);

	}

	std::vector<Intersection<T>> intersectObject(const std::shared_ptr<Object3D<T>>& o, bool recursive) const {

		std::vector<Intersection<T>> intersects;

		intersectObject(o, this, intersects, recursive);

		std::sort(intersects.front(), intersects.back(), ascSort);

		return intersects;

	}

	static Vector3<T> uvIntersection(const Vector3<T>& point, const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3, Vector3<T> uv1, Vector3<T> uv2, Vector3<T> uv3) {

		Vector3<T> barycoord = Triangle<T>::barycoordFromPoint(point, p1, p2, p3);

		uv1 *= barycoord.x;
		uv2 *= barycoord.y;
		uv3 *= barycoord.z;

		uv1 += uv2 + uv3;

		return uv1;

	}

	std::unique_ptr<Intersection<T>> checkIntersection(std::shared_ptr<Object3D<T>> o, SideConstant side,
			const Vector3<T>& pA, const Vector3<T>& pB, const Vector3<T>& pC) const {

		std::shared_ptr<Vector3<T>> intersect;

		if (side == BackSide){
			intersect = ray.intersectTriangle(pC, pB, pA, true);
		} else {
			intersect = ray.intersectTriangle(pA, pB, pC, side != DoubleSide);
		}

		if (intersect == nullptr) return nullptr;

		intersect.applyMatrix4(o->matrixWorld);

		T distance = ray.origin.distanceTo(intersect);

		if (distance < near || distance > far) return nullptr;

		auto intersection = std::make_unique<Intersection<T>>();

		intersection->distance = distance;
		intersection->point = intersect;
		intersection->object = o;

		return intersection;

	}

};

#endif

