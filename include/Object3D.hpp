
#ifndef MyUPlay_MyEngine_Object3D
#define MyUPlay_MyEngine_Object3D

#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <memory>
#include <algorithm>

#include "Vector3.hpp"
#include "Euler.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include "Raycaster.hpp"
#include "Log.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Object3D {
		private:

			Log logger("Object3D");

		public:

			const Math::UUID id = Math::generateUUID();
			std::string name;

			std::weak_ptr<Object3D> parent; //If the parent is destroyed, we probably will be too.

			std::vector<std::shared_ptr<Object3D> > children;

			Vector3<T> up(0, 1, 0);

			Vector3<T> position;
			Euler<T> rotation;
			Quaternion<T> quaternion;
			Vector3<T> scale(1, 1, 1);

			bool rotationAutoUpdate = true;
			Matrix4<T> matrix;
			Matrix4<T> matrixWorld;

			bool matrixAutoUpdate = true;
			bool matrixWorldNeedsUpdate = false;

			bool visible = true;

			bool castShadow = false;
			bool receiveShadow = false;

			bool frustumCulled = true;
			unsigned renderOrder = 0;

			Object3D(){}
			~Object3D(){}

			Object3D(std::shared_ptr<Object3D> o){
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
				Quaternion q1;
				q1.setFromAxisAngle(axis, angle);
				quaterion *= q1;
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
				Vector3 v1(axis);
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

			/**
			 * A helper function for easily adding multiple objects.
			 */
			Object3D& add(std::shared_ptr<Object3D> self, std::shared_ptr<Object3D> object, std::shared_ptr<Object3D>... objects){
				add(self, Object); //Add first object
				add(self, objects); //Repeat without the first object.
				return *this;
			}

			/**
			 * This function requires its shared_ptr to be pased into
			 * the function. All of the objects added must reference the correct
			 * shared_ptr to avoid early deletion.
			 */
			virtual Object3D& add(std::shared_ptr<Object3D> self, std::shared_ptr<Object3D> object){
				if (object == this){
					logger.error("Add: Object cannot be added to itself.");
					return *this;
				}

				if (!object->parent.expired()){
					object->parent.lock()->remove(object);
				}

				object->parent = self;
				children.push_back(object);

				return *this;
			}

			Object3D& add(std::shared_ptr<Object3D> self, std::shared_ptr<Object3D> object, std::shared_ptr<Object3D>... objects){
				add(self, object);
				add(self, objects);
				return *this;
			}

			virtual Object3D& remove(sdt::shared_ptr<Object3D> self, std::shared_ptr<Object3D> object){

				auto loc = std::find(children.begin(), children.end(), object);

				if (loc != children.end()){
					object.parent = std::shared_ptr<Object3D>(); //Reset parent pointer to null;
					object.erase(loc);
				}

				return *this;

			}

			std::shared_ptr<Object3D> getObjectById(const Math::UUID& id) const {
				for (auto o : children) {
					if (o->uuid = id){
						return o;
					}

					auto o2 = o->getObjectById(id);

					if (o2){
						return o2;
					}
				}
				return NULL;
			}
			inline Object3D& getObjectByName(std::string&& s) const { //Test move version.
				return getObjectByName(s);
			}
			Object3D& getObjectByName(const std::string& name) const {
				for (auto o : children) {
					if (o->name = name){
						return o;
					}

					auto o2 = o->getObjectByName(name);

					if (o2){
						return o2;
					}
				}
				return NULL;
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
				return result.setFromQuaternion(q, rotation.order, false);
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
				return result.set(0, 0, 1).applyQuaternion(q);
			}

			Object3D& traverse(std::function<void(std::shared_ptr<Object3D> self, std::shared_ptr<Object3D>)> func){
				func(self);
				for (auto o : children){
					o.traverse(o, func);
				}
				return *this;
			}
			Object3D& traverseVisible(std::function<void(std::shared_ptr<Object3D> self, std::shared_ptr<Object3D>)> func){
				if (visible){
					func(self);
					for (auto o : children){
						o.traverseVisible(o, func);
					}
				}
				return *this;
			}
			Object3D& traverseAnsestors(std::function<void(std::shared_ptr<Object3D> self, std::shared_ptr<Object3D>)> func){
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

					if (parent.expired()) {
						matrixWorld = matrix;
					} else {
						matrixWorld = parent.lock()->matrixWorld * matrix;
					}

					matrixWorldNeedsUpdate = false;

					force = true;

				}

				for (auto o : children) {
					o.updateMatrixWorld(force);
				}

				return *this;

			}

			Object3D& copy(std::shared_ptr<Object3D> o, bool recursive = true) {
				name = o->name;
				up = o->up;

				position = o->position;
				quaternion = o->quaternion;
				scale = o->scale;

				matrix = o->matrix;
				matrixWorld = o->matrixWorld;

				matrixAutoUpdate = o->matrixAutoUpdate;
				matrixWorldNeedsUpdate = o->matrixWorldNeedsUpdate;

				visible = o->visible;

				castShadow = o->castShadow;
				receiveShadow = o->receiveShadow;

				frustumCulled = o->frustumCulled;
				renderOrder = o->renderOrder;

				if (recursive){
					for (auto o : children){
						add(new Object3D(o));
					}
				}

				return *this;

			}
			Object3D clone(bool recursive = false) const {
				return Object3D().copy(*this, recursive);
			}
			Object3D clone(const Object3D& o, bool recursive = false) const {
				return Object3D().copy(o, recursive);
			}
			Object3D& operator=(const Object3D& o){
				return copy(o);
			}

			bool operator==(const Object3D& o){
				return id == o.id;
			}

			virtual void raycast(const Raycaster<T>&, std::vector<Intersection<T> >&) const {}

		};

		#define OBJECT3D_DEFINED

	}

}

#endif
