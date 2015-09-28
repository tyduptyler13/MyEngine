
#ifndef MyUPlay_MyEngine_Object3D
#define MyUPlay_MyEngine_Object3D

#include <string>
#include <vector>
#include <functional>
#include <mutex>

#include "Vector3.hpp"
#include "Euler.hpp"
#include "Quanternion.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Object3D {

			public:

				static unsigned idCount;

				const Math::UUID id = Math::generateUUID();
				std::string name;

				Object3D* parent;

				std::vector<Object3D*> children;

				Vector3f up;

				static const Vector3f DefaultUp;

				Vector3f position;
				Euler rotation;
				Quaternion quaternion;
				Vector3f scale;

				bool rotationAutoUpdate = true;
				Matrix4 matrix;
				Matrix4 matrixWorld;

				bool matrixAutoUpdate = true;
				bool matrixWorldNeedsUpdate = false;

				bool visible = true;

				bool castShadow = false;
				bool receiveShadow = false;

				bool frustumCulled = true;
				unsigned renderOrder = 0;

				Object3D();
				~Object3D();

				Object3D& applyMatrix(const Matrix4&);

				Object3D& setRotationAxisAngle(const Vector3f& axis, float angle);
				Object3D& setRotationFromEuler(const Euler&);
				Object3D& setRotationFromQuaternion(const Quaternion&);

				Object3D& rotateOnAxis(Vector3f axis, float angle);
				Object3D& rotateX(float angle);
				Object3D& rotateY(float angle);
				Object3D& rotateZ(float angle);

				Object3D& translateOnAxis(Vector3f axis, float distance);
				Object3D& translateX(float distance);
				Object3D& translateY(float distance);
				Object3D& translateZ(float distance);
				
				Vector3f& localToWorld(Vector3f);
				Vector3f& worldToLocal(Vector3f);

				Object3D& lookAt(Vector3f);

				Object3D& add(Object3D);
				Object3D& remove(Object3D);

				Object3D& getObjectById(unsigned id);
				Object3D& getObjectByName(std::string name);

				Vector3f* getWorldPosition(Vector3f* target = new Vector3f());
				Quaternion* getWorldQuaternion(Quaternion* target = new Quaternion());
				Euler* getWorldRotation(Euler* target = new Euler());
				Vector3f* getWorldScale(Vector3f* target = new Vector3f());
				Vector3f* getWorldDirection(Vector3f* target = new Vector3f());

				Object3D& traverse(std::function<void(Object3D&)>);
				Object3D& traverseAnsestors(std::function<void(Object3D&)>);

				Object3D& updateMatrix();

				Object3D& updateMatrixWorld(bool force = false);

				Object3D& clone(const Object3D&, bool recursive = false);

				Object3D& operator=(const Object3D& o){
					return clone(o);
				}

				bool operator==(const Object3D& o){
					return id == o.id;
				}

		};

	}

}

#endif

