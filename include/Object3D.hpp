#ifndef MYUPLAY_MYENGINE_OBJECT3D
#define MYUPLAY_MYENGINE_OBJECT3D

#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <memory>
#include <algorithm>

namespace MyUPlay {
	namespace MyEngine {
		template <typename T> class Object3D;
	}
}

#include "Vector3.hpp"
#include "Euler.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include "Raycaster.hpp"
#include "Log.hpp"

template <typename T = float>
class MyUPlay::MyEngine::Object3D {
private:

	Log logger = Log("Object3D");

public:

	enum ObjectType { //The following are the supported types for objects. Each has different rendering properties.
		BASIC, //A container, does not render
		LIGHT, //Does not render but can be applied in shading
		SCENE, //Top container (shouldn't be a child) (Might be redundant)
		MESH, //Basic renderable object (MUST BE OF TYPE Mesh!)
		SPRITE, //Renders a flat object in 3d space
		LENSFLARE, //Applied after rendering
		LINE, //Renders an object in line mode (wireframe) You can also use the wireframe property in some materials
		POINTS //Renders points with no lines or faces (usually with circle sprites)
	};

	ObjectType type;

	static unsigned object3DIdCounter;

	const unsigned id = object3DIdCounter++;
	const Math::UUID uuid = Math::generateUUID();
	std::string name;

	Object3D* parent = nullptr; //We don't want to keep a ref count on something that also owns us.

	/**
	 * All children are unique. Copies may be made, but no copies of pointers.
	 */
	std::vector<std::unique_ptr<Object3D> > children;

	Vector3<T> up = Vector3<T>(0, 1, 0);

	Vector3<T> position;
	Euler<T> rotation;
	Quaternion<T> quaternion;
	Vector3<T> scale = Vector3<T>(1, 1, 1);

	bool rotationAutoUpdate = true;
	Matrix4<T> matrix;
	Matrix4<T> matrixWorld;

	Matrix4<T> modelViewMatrix;
	Matrix3<T> normalMatrix;

	bool matrixAutoUpdate = true;
	bool matrixWorldNeedsUpdate = false;

	bool visible = true;

	bool castShadow = false;
	bool receiveShadow = false;

	bool frustumCulled = true;
	unsigned renderOrder = 0;

	Object3D(ObjectType t = BASIC) : type(t) {}
	virtual ~Object3D(){}

	Object3D(const Object3D& o){
		copy(o);
	}

	Object3D& applyMatrix(const Matrix4<T>& m) {
		matrix = m * matrix;
		matrix.decompose(position, quaternion, scale);
		return *this;
	}

	Object3D& setRotationAxisAngle(const Vector3<T>& axis, float angle){
		quaternion.setFromAxisAngle(axis, angle);
		return *this;
	}
	Object3D& setRotationFromEuler(const Euler<T>& e){
		quaternion.setFromEuler(e, true);
		return *this;
	}
	Object3D& setRotationFromMatrix(const Matrix4<T>& m){
		quaternion.setFromRotationMatrix(m);
		return *this;
	}
	Object3D& setRotationFromQuaternion(const Quaternion<T>& q){
		quaternion = q;
		return *this;
	}

	Object3D& rotateOnAxis(Vector3<T> axis, float angle) {
		Quaternion<T> q1;
		q1.setFromAxisAngle(axis, angle);
		quaternion *= q1;
		return *this;
	}
	Object3D& rotateX(float angle) {
		const Vector3<T> v1(1, 0, 0);
		rotateOnAxis(v1, angle);
		return *this;
	}
	Object3D& rotateY(float angle){
		const Vector3<T> v1(0, 1, 0);
		rotateOnAxis(v1, angle);
		return *this;
	}
	Object3D& rotateZ(float angle){
		const Vector3<T> v1(0, 0, 1);
		rotateOnAxis(v1, angle);
		return *this;
	}

	Object3D& translateOnAxis(Vector3<T> axis, float distance){
		Vector3<T> v1(axis);
		v1.applyQuaternion(quaternion);
		position += v1 * distance;
		return *this;
	}
	Object3D& translateX(float distance){
		const Vector3<T> v1(1, 0, 0);
		translateOnAxis(v1, distance);
		return *this;
	}
	Object3D& translateY(float distance){
		const Vector3<T> v1(0, 1, 0);
		translateOnAxis(v1, distance);
		return *this;
	}
	Object3D& translateZ(float distance){
		const Vector3<T> v1(0, 0, 1);
		translateOnAxis(v1, distance);
		return *this;
	}

	Vector3<T>& localToWorld(Vector3<T>& vector) const {
		return vector.applyMatrix4(matrixWorld);
	}
	Vector3<T>& worldToLocal(Vector3<T>& vector) const {
		return vector.applyMatrix4(Matrix4<T>().getInverse(matrixWorld));
	}

	Object3D& lookAt(const Vector3<T>& vector){
		Matrix4<T> m1 = Matrix4<T>().lookAt(vector, position, up);
		quaternion.setFromRotationMatrix(m1);
		return *this;
	}

	Object3D& add(std::vector<Object3D*> objects){
		for (Object3D* o : objects){
			add(o);
		}
		return *this;
	}

	Object3D& add(Object3D* object){
		if (object->parent != nullptr){
			object->parent->remove(object);
		}

		object->parent = this;
		children.push_back(object);

		return *this;
	}

	Object3D& remove(std::vector<Object3D*> objects){
		for (Object3D* o : objects){
			remove(o);
		}
		return *this;
	}

	/**
	 * Removes a child from this object.
	 *
	 * WARNING: Removing an object will automatically free its memory.
	 *
	 * By setting release to true, you assume responsibility for freeing the objects memory later.
	 */
	Object3D& remove(Object3D* object, bool release = false){

		auto loc = std::find(children.begin(), children.end(), object);

		if (loc != children.end()){
			object->parent = nullptr; //Reset parent pointer to null;
			if (release){
				loc->release();
			}
			children.erase(loc);
		}

		return *this;

	}

	Object3D* getObjectById(const Math::UUID& id) const {
		for (auto o : children) {
			if (o->uuid == id){
				return o;
			}

			auto o2 = o->getObjectById(id);

			if (o2){
				return o2;
			}
		}
		return nullptr;
	}
	inline Object3D* getObjectByName(std::string&& s) const { //Test move version.
		return getObjectByName(s);
	}
	Object3D* getObjectByName(const std::string& name) const {
		for (auto o : children) {
			if (o->name == name){
				return o;
			}

			auto o2 = o->getObjectByName(name);

			if (o2){
				return o2;
			}
		}
		return nullptr;
	}

	Vector3<T> getWorldPosition() {
		Vector3<T> v1;
		return getWorldPosition(v1);
	}
	Quaternion<T> getWorldQuaternion() {
		Quaternion<T> q;
		return getWorldQuaternion(q);
	}
	Euler<T> getWorldRotation() {
		Euler<T> e;
		return getWorldRotation(e);
	}
	Vector3<T> getWorldScale() {
		Vector3<T> v1;
		return getWorldScale(v1);
	}
	Vector3<T> getWorldDirection() {
		Vector3<T> v1;
		return getWorldDirection(v1);
	}

	Vector3<T>& getWorldPosition(Vector3<T>& target) {
		updateMatrixWorld(true);
		return target.setFromMatrixPosition(matrixWorld);
	}
	Quaternion<T>& getWorldQuaternion(Quaternion<T>& target) {
		updateMatrixWorld(true);
		Vector3<T> pos, scale;
		matrixWorld.decompose(pos, target, scale);
		return target;
	}
	Euler<T>& getWorldRotation(Euler<T>& target) {
		Quaternion<T> q = getWorldQuaternion();
		return target.setFromQuaternion(q, rotation.order, false);
	}
	Vector3<T>& getWorldScale(Vector3<T>& target) {
		Quaternion<T> q;
		Vector3<T> pos;
		updateMatrixWorld(true);
		matrixWorld.decompose(pos, q, target);
		return target;
	}
	Vector3<T>& getWorldDirection(Vector3<T>& target) {
		Quaternion<T> q = getWorldQuaternion();
		return target.set(0, 0, 1).applyQuaternion(q);
	}

	Object3D& traverse(Object3D* self, std::function<void(Object3D*)> func){
		func(self);
		for (auto o : children){
			o->traverse(o, func);
		}
		return *this;
	}
	Object3D& traverseVisible(Object3D* self, std::function<void(Object3D*)> func){
		if (visible){
			func(self);
			for (auto o : children){
				o->traverseVisible(o, func);
			}
		}
		return *this;
	}
	Object3D& traverseAnsestors(Object3D* self, std::function<void(Object3D*)> func){
		func(self);
		for (auto o : children){
			o->traverseAnsestors(o, func);
		}
		return *this;
	}

	Object3D& updateMatrix() {

		matrix.compose(position, quaternion, scale);

		matrixWorldNeedsUpdate = true;

		return *this;

	}

	Object3D& updateMatrixWorld(bool force = false) {

		if (matrixAutoUpdate) updateMatrix();

		if (matrixWorldNeedsUpdate || force) {

			if (parent == nullptr) {
				matrixWorld = matrix;
			} else {
				matrixWorld = parent->matrixWorld * matrix;
			}

			matrixWorldNeedsUpdate = false;

			force = true;

		}

		for (auto& o : children) {
			o->updateMatrixWorld(force);
		}

		return *this;

	}

	Object3D& copy(const Object3D& o, bool recursive = true) {
		name = o.name;
		up = o.up;

		position = o.position;
		quaternion = o.quaternion;
		scale = o.scale;

		matrix = o.matrix;
		matrixWorld = o.matrixWorld;

		matrixAutoUpdate = o.matrixAutoUpdate;
		matrixWorldNeedsUpdate = o.matrixWorldNeedsUpdate;

		visible = o.visible;

		castShadow = o.castShadow;
		receiveShadow = o.receiveShadow;

		frustumCulled = o.frustumCulled;
		renderOrder = o.renderOrder;

		if (recursive){
			for (auto o : children){
				add(new Object3D(o));
			}
		}

		return *this;

	}
	Object3D& operator=(const Object3D& o){
		return copy(o);
	}

	bool operator==(const Object3D& o){
		return id == o.id;
	}

	virtual void raycast(const Raycaster<T>&, std::vector<Intersection<T> >&) const {} //TODO

};

template <typename T>
unsigned MyUPlay::MyEngine::Object3D<T>::object3DIdCounter = 0;

#endif
