
#ifndef MyUPlay_MyEngine_Object3D
#define MyUPlay_MyEngine_Object3D

#include <string>
#include <vector>

#include "Vector3.hpp"
#include "Euler.hpp"
#include "Quanternion.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Object3D {

			public:

				uuid id;
				std::string name;

				Object3D* parent;

				std::vector<Object3D*> children;

				Vector3 up;

				static const Vector3 DefaultUp;

				Vector3 position;
				Euler rotation;
				Quaternion quaternion;
				Vector3 scale;

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

				Object3D& applyMatrix(Matrix4);

				Object3D& setRotationAxisAngle(Vector3 axis, float angle);
				Object3D& setRotationFromEuler(Euler);
				Object3D& setRotationFromQuaternion(const Quaternion&);

				Object3D& rotateOnAxis(Vector3 axis, float angle);
				Object3D& rotateX(float angle);
				Object3D& rotateY(float angle);
				Object3D& rotateZ(float angle);

				Object3D& translateOnAxis(Vector3 axis, float distance);
				Object3D& translateX(float distance);
				Object3D& translateY(float distance);
				Object3D& translateZ(float distance);
				
				Vector3& localToWorld(Vector3);
				Vector3& worldToLocal(Vector3);

				Object3D& lookAt(Vector3);

				Object3D& add(Object3D& );
				Object3D& remove(Object3D);

				Object3D& getObjectById(uuid id);
				Object3D& getObjectByName(std::string name);

				Vector3* getWorldPosition(Vector3* target = new Vector3());
				Quaternion* getWorldQuaternion(Quaternion* target = new Quaternion());
				Euler* getWorldRotation(Euler* target = new Euler());
				Vector3* getWorldScale(Vector3* target = new Vector3());
				Vector3* getWorldDirection(Vector3* target = new Vector3());






		};

	}

}

#endif

