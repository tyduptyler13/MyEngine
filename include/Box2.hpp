
#ifndef MYUPLAY_MYENGINE_BOX2
#define MYUPLAY_MYENGINE_BOX2

#include <vector>
#include <limits>

#include "Vector2.hpp"


namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Box2 {

			private:
			typedef std::numeric_limits<T> limit;

			public:

			Vector2<T> min;
			Vector2<T> max;

			Box2(){
				min(limit::infinity(), limit::infinity());
				max(-limit::infinity(), -limit::infinity());

			}
			Box2(Vector2<T> min, Vector2<T> max) :min(min), max(max){}
			Box2(const Vector2<T>& min, const Vector2<T>& max): min(min), max(max){}

			Box2(const Box2& b) : min(b.min), max(b.max){}

			Box2& set(Vector2<T> min, Vector2<T> max){
				return set(min, max);
			}

			Box2& set(const Vector2<T>& min, const Vector2<T>& max){
				this->min(min);
				this->max(max);
				return *this;
			}

			Box2& setFromPoints(const std::vector<Vector2<T>>& points){

				makeEmpty();

				for (const Vector2<T>& point : points){
					expandByPoint(point);
				}

				return *this;

			}

			Box2& setFromCenterAndSize(const Vector2<T>& center, const Vector2<T>& size){

				Vector2<T> halfSize = size * 0.5;
				min = center - halfSize;
				max = center + halfSize;

				return *this;

			}

			inline Box2& operator=(const Box2& b){
				return copy(b);
			}

			Box2& copy(const Box2& b){
				min(b.min);
				max(b.max);
				return *this;
			}

			Box2& makeEmpty(){
				min.set(limit::infinity(), limit::infinity());
				max.set(-limit::infinity(), -limit::infinity());
				return *this;
			}

			bool empty(){
				return (max.x < min.x) || (max.y < min.y);
			}

			inline Vector2<T> center(){
				return center(Vector2<T>());
			}

			Vector2<T>& center(Vector2<T>& target){
				return target.addVectors(min, max) *= .5;
			}

			inline Vector2<T> size(){
				return size(Vector2<T>());
			}

			Vector2<T>& size(Vector2<T> target){
				return target.subVectors(min, max);
			}

			Box2& expandByPoint(const Vector2<T>& point){

				min.min(point);
				max.max(point);
				return *this;

			}

			Box2& expandByVector(const Vector2<T>& vector){

				min -= vector;
				max += vector;
				return *this;

			}

			Box2& expandByScalar(T scalar){

				min -= scalar;
				max += scalar;

				return *this;

			}

			bool containsPoint(const Vector2<T>& point){

				if (point.x < min.x || point.x > max.x ||
				    point.y < min.y || point.y > max.y) {

					return false;

				}

				return true;

			}

			bool containsBox(const Box2& box){

				if (min.x <= box.min.x && box.max.x <= max.x &&
				    min.y <= box.min.y && box.max.y <= max.y) {

					return true;

				}

				return false;

			}

			inline Vector2<T> getParameter(const Vector2<T>& point){
				return getParameter(point, Vector2<T>());
			}

			Vector2<T>& getParameter(const Vector2<T>& point, Vector2<T>& target){
				return target.set(
					(point.x - min.x) / (max.x - min.x),
					(point.y - min.y) / (max.y - min.y)
				);
			}

			bool isIntersectionBox(const Box2& box){

				if (box.max.x < min.x || box.min.x > max.x ||
				    box.max.y < min.y || box.min.y > max.y) {
					return false;
				}
				return true;

			}

			inline Vector2<T> clampPoint(const Vector2<T>& point){
				return clampPoint(point, Vector2<T>());
			}

			Vector2<T>& clampPoint(const Vector2<T>& point, Vector2<T>& target){
				return target.copy(point).clamp(min, max);
			}

			T distanceToPoint(Vector2<T> point){
				return point.clamp(min, max).sub(point).length();
			}

			Box2& intersect(const Box2& box){

				min.max(box.min);
				max.min(box.max);

				return *this;

			}

			//union is a reserved word
			Box2& boxUnion(const Box2& box){

				min.min(box.min);
				max.max(box.max);

				return *this;

			}

			Box2& translate(const Vector2<T>& offset){

				min += offset;
				max += offset;

				return *this;

			}

			inline bool operator==(const Box2& b){
				return equals(b);
			}

			bool equals(const Box2& b){
				return b.min == min && b.max == max;
			}


		};

		typedef Box2<float> Box2f;
		typedef Box2<double> Box2d;

	}

}

#endif

