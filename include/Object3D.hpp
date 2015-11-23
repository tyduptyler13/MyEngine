
#ifndef MyUPlay_MyEngine_Object3D
#define MyUPlay_MyEngine_Object3D

#include <string>
#include <vector>
#include <functional>
#include <mutex>

#include "Vector3.hpp"
#include "Euler.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Object3D {

		public:

			const Math::UUID id = Math::generateUUID();
			std::string name;

			Object3D* parent;

			std::vector<Object3D*> children;

			Vector3<T> up;

			static const Vector3<T> DefaultUp;

			Vector3<T> position;
			Euler<T> rotation;
			Quaternion<T> quaternion;
			Vector3<T> scale;

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

			Object3D();
			~Object3D();

			Object3D& applyMatrix(const Matrix4<T>&);

			Object3D& setRotationAxisAngle(const Vector3<T>& axis, float angle);
			Object3D& setRotationFromEuler(const Euler<T>&);
			Object3D& setRotationFromQuaternion(const Quaternion<T>&);

			Object3D& rotateOnAxis(Vector3<T> axis, float angle);
			Object3D& rotateX(float angle);
			Object3D& rotateY(float angle);
			Object3D& rotateZ(float angle);

			Object3D& translateOnAxis(Vector3<T> axis, float distance);
			Object3D& translateX(float distance);
			Object3D& translateY(float distance);
			Object3D& translateZ(float distance);
			
			Vector3<T>& localToWorld(Vector3<T>&);
			Vector3<T>& worldToLocal(Vector3<T>&);

			Object3D& lookAt(const Vector3<T>&);

			Object3D& add(Object3D);
			Object3D& remove(Object3D);

			Object3D& getObjectById(std::string id) const;
			Object3D& getObjectByName(std::string name) const;
			
			Vector3<T> getWorldPosition() const{
				return getWorldPosition(Vector3<T>());
			}
			Quaternion<T> getWorldQuaternion() const {
				return getWorldQuaternion(Quaternion<T>());
			}
			Euler<T> getWorldRotation() const {
				return getWorldRotation(Euler<T>());
			}
			Vector3<T> getWorldScale() const {
				return getWorldScale(Vector3<T>());
			}
			Vector3<T> getWorldDirection() const {
				return getWorldDirection(Vector3<T>());
			}

			Vector3<T>& getWorldPosition(Vector3<T>& target) const;
			Quaternion<T>& getWorldQuaternion(Quaternion<T>& target) const;
			Euler<T>& getWorldRotation(Euler<T>& target) const;
			Vector3<T>& getWorldScale(Vector3<T>& target) const;
			Vector3<T>& getWorldDirection(Vector3<T>& target) const;

			Object3D& traverse(std::function<void(Object3D&)>);
			Object3D& traverse(std::function<void(const Object3D&)>) const;
			Object3D& traverseAnsestors(std::function<void(Object3D&)>);
			Object3D& traverseAnsestors(std::function<void(const Object3D&)>) const;

			Object3D& updateMatrix();

			Object3D& updateMatrixWorld(bool force = false);

			Object3D& copy(const Object3D&, bool recursive = false);
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

		};

		#define OBJECT3D_DEFINED

	}

}

#endif

