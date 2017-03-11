#ifndef MYUPLAY_MYENGINE_GEOMETRY
#define MYUPLAY_MYENGINE_GEOMETRY

#include <vector>
#include <string>
#include <memory>
#include <string>

namespace MyUPlay {
	namespace MyEngine {

		template <typename T>
		struct IGeometry;

	}
}

#include "Math.hpp"
//#include "Color.hpp" //TODO Implement per vertex colors.
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Box3.hpp"
#include "Sphere.hpp"
#include "Matrix4.hpp"
#include "Raycaster.hpp"
#include "Constants.hpp"
#include "BufferAttribute.hpp"

/**
 * Geometry Interface, for pointers that don't care about the underlying geometry.
 */
template <typename T>
struct MyUPlay::MyEngine::IGeometry {

	struct Group {
		unsigned start,
		count,
		materialIndex;
		Group(unsigned start, unsigned count, unsigned materialIndex)
		: start(start), count(count), materialIndex(materialIndex) {}
	};

	Math::UUID uuid = Math::generateUUID();

	std::string name;

	std::unique_ptr<Box3<T> > boundingBox;
	std::unique_ptr<Sphere<T> > boundingSphere;

	bool multiMaterial = false;

	bool boundingBoxDirty = true, //Must be set to true for it to recalculate
		boundingSphereDirty = true; //Must be set to true for it to recalculate

	virtual void applyMatrix(const Matrix4<T>& matrix) = 0;

	virtual void computeBoundingBox() = 0;
	virtual void computeBoundingSphere() = 0;

	virtual bool isBufferGeometry() const {
		return false;
	}

	virtual BufferAttribute<T>& getPositions() const = 0;

	virtual BufferAttribute<T>& getNormals() const = 0;

	/**
	 * Returns the number elements in the Geometry.
	 *
	 * An element is a single set of Vertex, Normal, and Color.
	 *
	 * To get faces divide this by 3. (All faces are triangles)
	 */
	virtual unsigned size() const = 0;

	bool isMultiMaterial() const {
		return multiMaterial;
	}
	virtual std::vector<Group> getGroups() const {
		return {};
	}

	virtual void raycast(std::shared_ptr<Object3D<T>>& obj, const Raycaster<T>& r, std::vector<Intersection<T>>& intersections, SideConstant s) const = 0;

	virtual ~IGeometry(){}

protected:

	IGeometry(){}
	IGeometry(std::string name) : name(name) {}

	IGeometry(const IGeometry<T>& g) : name(g.name) {}

	IGeometry(IGeometry<T>&& g) : name(std::move(g.name)) {}

public:

	/*
	 * The following functions do permanent manipulations of the actual points.
	 * This is sometimes called baked transformations.
	 */

	IGeometry* rotateX(T angle) {
		Matrix4<T> m1;
		m1.makeRotationX(angle);
		this->applyMatrix(m1);
		return this;
	}

	IGeometry* rotateY(T angle) {
		Matrix4<T> m1;
		m1.makeRotationY(angle);
		this->applyMatrix(m1);
		return this;
	}

	IGeometry* rotateZ(T angle) {
		Matrix4<T> m1;
		m1.makeRotationZ(angle);
		this->applyMatrix(m1);
		return this;
	}

	IGeometry* translate(T x, T y, T z) {
		Matrix4<T> m1;
		m1.makeTranslation(x, y, z);
		this->applyMatrix(m1);
		return this;
	}

	IGeometry* scale(T x, T y, T z) {
		Matrix4<T> m1;
		m1.makeScale(x, y, z);
		this->applyMatrix(m1);
		return this;
	}

	IGeometry* lookAt(const Vector3<T>& v){
		Object3D<T> obj;
		obj.lookAt(v);
		obj.updateMatrix();
		this->applyMatrix(obj.matrix);
		return this;
	}

	Vector3<T> center(){
		this->computeBoundingBox();
		Vector3<T> offset = this->boundingBox->center().negate();
		translate(offset.x, offset.y, offset.z);
		return offset;
	}

	IGeometry* normalize(){
		this->computeBoundingSphere();

		Vector3<T> center = this->boundingSphere->center;
		T radius = this->boundingSphere->radius;

		T s = radius == 0 ? 1 : 1.0 / radius;

		Matrix4<T> matrix;
		matrix.set(
				s, 0, 0, - s * center.x,
				0, s, 0, - s * center.y,
				0, 0, s, - s * center.z,
				0, 0, 0, 1
		);

		this->applyMatrix( matrix );

		return this;
	}

};

#endif
