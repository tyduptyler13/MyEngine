#ifndef MYUPLAY_MYENGINE_BOX2
#define MYUPLAY_MYENGINE_BOX2

#include "Vector2.hpp"
#include "Box.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Box2 : public Box<Vector2, T, Box2<T> > {

			protected:
			typedef Box<Vector2, T, Box2<T> > B;
			typedef typename B::limit limit;

			public:

			//These are required because min/max is common in std (math headers)
			Vector2<T>& min = B::min;
			Vector2<T>& max = B::max;

			Box2(){
				min(limit::infinity(), limit::infinity());
				max(-limit::infinity(), -limit::infinity());

			}
			Box2(Vector2<T> min, Vector2<T> max) : B(min, max) {}
			Box2(const Vector2<T>& min, const Vector2<T>& max) : B(min, max) {}

			Box2(const Box2& b) : B(b) {}

			Box2& makeEmpty(){
				min.set(limit::infinity(), limit::infinity());
				max.set(-limit::infinity(), -limit::infinity());
				return *this;
			}

			bool empty(){
				return (max.x < min.x) || (max.y < min.y);
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

		};

		typedef Box2<float> Box2f;
		typedef Box2<double> Box2d;

	}

}

#endif

