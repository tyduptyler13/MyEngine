#ifndef MYUPLAY_MYENGINE_BOX3
#define MYUPLAY_MYENGINE_BOX3

#include <limits>
#include <vector>

#include "Vector3.hpp"
#include "Object3D.hpp"
#include "Sphere.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef SPHERE_DEFINED
		template <typename T> class Sphere;
		#endif
		#ifndef OBJECT3D_DEFINED
		template <typename T> class Object3D;
		#endif
		#ifndef VECTOR3_DEFINED
		template <typename T> class Vector3;
		#endif

		template <typename T>
		class Box3 {

			protected:
			//Shorthand tricks.

			typedef std::numeric_limits<T> limit;

			public:

			Vector3<T> min;
			Vector3<T> max;

			Box3() :
				min(limit::infinity(), limit::infinity(), limit::infinity()),
				max(-limit::infinity(), -limit::infinity(), -limit::infinity())
			{}

			Box3(Vector3<T> min, Vector3<T> max) : min(min), max(max) {}
			Box3(const Vector3<T>& min, const Vector3<T>& max) : min(min), max(max){}

			Box3(const Box3& b) : min(b.min), max(b.max) {}

			Box3& makeEmpty()  {
				min.set(limit::infinity(), limit::infinity(), limit::infinity());
				max.set(-limit::infinity(), -limit::infinity(), -limit::infinity());
				return *this;
			}

			Box3& set(const Vector3<T>& min, const Vector3<T>& max) {
                this->min = min;
                this->max = max;
                return *this;
            }

            Box3& setFromPoints(const std::vector<Vector3<T> >& points){
                makeEmpty();
                for (const Vector3<T>& point : points){
                    expandByPoint(point);
                }
                return *this;
            }

            Box3& setFromCenterAndSize(const Vector3<T>& center, const Vector3<T>& size){
                Vector3<T> halfSize = size * 0.5;
                min = center - halfSize;
                max = center + halfSize;
                return *this;
            }

			bool empty()  {
				return (max.x < min.x) || (max.y < min.y) || (max.z < min.z);
			}

			Box3& setFromObject(Object3D<T>& object){

				object.updateMatrixWorld(true);

				makeEmpty();

				object.traverse([this](Object3D<T>& node) {

					//This function assumes we don't have BufferGeometry. Everything is already native.
					for (Vector3<T> vertex : node.geometry.verticies) {

						vertex.applyMatrix4( node.matrixWorld );

						expandByPoint( vertex );

					}

				});

				return *this;

			}

			bool containsPoint(const Vector3<T>& point)  {

				if (point.x < min.x || point.x > max.x ||
				    point.y < min.y || point.y > max.y ||
				    point.z < min.z || point.z > max.z) {

					return false;

				}

				return true;

			}

			bool containsBox(const Box3& box)  {

				if (( min.x <= box.min.x ) && ( box.max.x <= max.x ) &&
				    ( min.y <= box.min.y ) && ( box.max.y <= max.y ) &&
				    ( min.z <= box.min.z ) && ( box.max.z <= max.z )) {

					return true;

				}

				return false;

			}

			Vector3<T>& getParameter(const Vector3<T>& point, Vector3<T>& target)  {

				return target.set(
						(point.x - min.x) / (max.x - min.x),
						(point.y - min.y) / (max.y - min.y),
						(point.z - min.z) / (max.z - min.z));

			}

			bool isIntersectionBox(const Box3& box)  {

				if ( box.max.x < min.x || box.min.x > max.x ||
				     box.max.y < min.y || box.min.y > max.y ||
				     box.max.z < min.z || box.min.z > max.z ) {

					return false;

				}

				return true;

			}

			Box3& applyMatrix4(const Matrix4<T>& matrix){

				std::vector<Vector3<T> > points(8);

				points[ 0 ].set( min.x, min.y, min.z ).applyMatrix4( matrix ); // 000
				points[ 1 ].set( min.x, min.y, max.z ).applyMatrix4( matrix ); // 001
				points[ 2 ].set( min.x, max.y, min.z ).applyMatrix4( matrix ); // 010
				points[ 3 ].set( min.x, max.y, max.z ).applyMatrix4( matrix ); // 011
				points[ 4 ].set( max.x, min.y, min.z ).applyMatrix4( matrix ); // 100
				points[ 5 ].set( max.x, min.y, max.z ).applyMatrix4( matrix ); // 101
				points[ 6 ].set( max.x, max.y, min.z ).applyMatrix4( matrix ); // 110
				points[ 7 ].set( max.x, max.y, max.z ).applyMatrix4( matrix );  // 111

				makeEmpty();
				setFromPoints( points );

				return *this;

			}

			Sphere<T> getBoundingSphere() {
				Sphere<T> s;
				return getBoundingSphere(s);
			}

			Sphere<T>& getBoundingSphere(Sphere<T>& target) {

				target.center = center();
				target.radius = size().length() * .5;

				return target;

			}

			inline Box3& operator=(const Box3& b){
				return copy(b);
			}

			Box3& copy(const Box3& b){
				min(b.min);
				max(b.max);
				return *this;
			}

			inline Vector3<T> center(){
				Vector3<T> v1;
				center(v1);
				return v1;
			}

			Vector3<T>& center(Vector3<T>& target){
				return target.addVectors(min, max).multiply(.5);
			}

			inline Vector3<T> size(){
				return size(Vector3<T>());
			}

			Vector3<T>& size(Vector3<T> target){
				return target.subVectors(min, max);
			}

			Box3& expandByPoint(const Vector3<T>& point){


				min.min(point);
				max.max(point);
				return *this;

			}

			Box3& expandByVector(const Vector3<T>& vector){

				min -= vector;
				max += vector;
				return *this;

			}

			Box3& expandByScalar(T scalar){

				min -= scalar;
				max += scalar;

				return *this;

			}

			inline Box3<T> getParameter(const Vector3<T>& point) const {
				return getParameter(point, Vector3<T>());
			}

			inline Vector3<T> clampPoint(const Vector3<T>& point) const {
				return clampPoint(point, Vector3<T>());
			}

			Vector3<T>& clampPoint(const Vector3<T>& point, Vector3<T>& target){
				return target.copy(point).clamp(min, max);
			}

			T distanceToPoint(Vector3<T> point) const {
				return point.clamp(min, max).sub(point).length();
			}

			Box3& intersect(const Box3& box){

				min.max(box.min);
				max.min(box.max);

				return *this;

			}

			//union is a reserved word
			Box3& boxUnion(const Box3& box){

				min.min(box.min);
				max.max(box.max);

				return *this;

			}

			Box3& translate(const Vector3<T>& offset){

				min += offset;
				max += offset;

				return *this;

			}

			inline bool operator==(const Box3& b) const {
				return equals(b);
			}

			bool equals(const Box3& b) const {
				return b.min == min && b.max == max;
			}

			void setFromArray(const std::vector<T>& array){
				T minX = limit::infinity();
				T minY = limit::infinity();
				T minZ = limit::infinity();

				T maxX = -limit::infinity();
				T maxY = -limit::infinity();
				T maxZ = -limit::infinity();

				for ( unsigned i = 0, l = array.size(); i < l; i += 3 ) {

					const T& x = array[ i ];
					const T& y = array[ i + 1 ];
					const T& z = array[ i + 2 ];

					if ( x < minX ) minX = x;
					if ( y < minY ) minY = y;
					if ( z < minZ ) minZ = z;

					if ( x > maxX ) maxX = x;
					if ( y > maxY ) maxY = y;
					if ( z > maxZ ) maxZ = z;

				}

				min.set( minX, minY, minZ );
				max.set( maxX, maxY, maxZ );
			}

		};

		typedef Box3<float> Box3f;
		typedef Box3<double> Box3d;

		#define BOX3_DEFINED

	}

}


#endif

