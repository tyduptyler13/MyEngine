#ifndef MYUPLAY_MYENGINE_BOX3
#define MYUPLAY_MYENGINE_BOX3

#include "Box.hpp"
#include "Vector3.hpp"


namespace MyUPlay {

	namespace MyEngine {
		
		template <typename T>
		class Box3 : public Box<Vector3, T, Box3<T>> {

			private:
			typedef Box<Vector3, T, Box3<T> > B;

			protected:
			typedef typename B::limit limit;

			public:

			Box3() {
				min(limit::infinity(), limit::infinity());
				max(-limit::infinity(), -limit::infinity());
			}

			Box3(Vector3<T> min, Vector3<T> max) : B(min, max) {}
			Box3(const Vector3<T>& min, const Vector3<T>& max) : B(min, max){}
			
			Box3(const Box3& b) : B(b) {}

			Box3& makeEmpty() {
				B::min.set(limit::infinity(), limit::infinity(), limit::infinity());
				B::max.set(-limit::infinity(), -limit::infinity(), -limit::infinity());
				return *this;
			}

			bool empty() {
				//TODO
			}


		};

	}

}


#endif

