
#ifndef MYUPLAY_MYENGINE_BOX
#define MYUPLAY_MYENGINE_BOX


#include <vector>
#include <limits>

namespace MyUPlay {

	namespace MyEngine {

		//D: dimensions (aka D or Vector3)
		//T: type accuracy that goes in D
		//B: child class for return types.
		template <template<typename> class D, typename T, typename B>
		class Box {

			public:
			D<T> min;
			D<T> max;

			private:
			Box(D<T> min, D<T> max): min(min), max(max) {}
			Box(const D<T>& min, const D<T>& max) : min(min), max(max) {}
			Box(const Box& b) : min(b.min), max(b.max) {}

			protected:
			typedef std::numeric_limits<T> limit;

			public:


			B& set(const D<T>& min, const D<T>& max){
				this->min(min);
				this->max(max);
				return *this;
			}

			B& setFromPoints(const std::vector< D<T> >& points){

				makeEmpty();

				for (const D<T>& point : points){
					expandByPoint(point);
				}

				return *this;

			}

			B& setFromCenterAndSize(const D<T>& center, const D<T>& size){

				D<T> halfSize = size * 0.5;
				min = center - halfSize;
				max = center + halfSize;

				return *this;

			}

			inline B& operator=(const B& b){
				return copy(b);
			}

			B& copy(const B& b){
				min(b.min);
				max(b.max);
				return *this;
			}

			virtual B& makeEmpty() = 0;

			virtual bool empty() = 0;

			inline D<T> center(){
				return center(D<T>());
			}

			D<T>& center(D<T>& target){
				return target.addVectors(min, max).mupltiply(.5);
			}

			inline D<T> size(){
				return size(D<T>());
			}

			D<T>& size(D<T> target){
				return target.subVectors(min, max);
			}

			B& expandByPoint(const D<T>& point){

				min.min(point);
				max.max(point);
				return *this;

			}

			B& expandByVector(const D<T>& vector){

				min -= vector;
				max += vector;
				return *this;

			}

			B& expandByScalar(T scalar){

				min -= scalar;
				max += scalar;

				return *this;

			}

			virtual bool containsPoint(const D<T>& point) = 0;

			virtual bool containsBox(const B& box) = 0;

			inline D<T> getParameter(const D<T>& point){
				return getParameter(point, D<T>());
			}

			virtual D<T>& getParameter(const D<T>& point, D<T>& target) = 0;

			virtual bool isIntersectionBox(const B& box) = 0;

			inline D<T> clampPoint(const D<T>& point){
				return clampPoint(point, D<T>());
			}

			D<T>& clampPoint(const D<T>& point, D<T>& target){
				return target.copy(point).clamp(min, max);
			}

			T distanceToPoint(D<T> point){
				return point.clamp(min, max).sub(point).length();
			}

			B& intersect(const B& box){

				min.max(box.min);
				max.min(box.max);

				return *this;

			}

			//union is a reserved word
			B& boxUnion(const B& box){

				min.min(box.min);
				max.max(box.max);

				return *this;

			}

			B& translate(const D<T>& offset){

				min += offset;
				max += offset;

				return *this;

			}

			inline bool operator==(const B& b){
				return equals(b);
			}

			bool equals(const B& b){
				return b.min == min && b.max == max;
			}


		};

	}

}

#endif

