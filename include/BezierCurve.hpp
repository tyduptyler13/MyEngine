#ifndef MYUPLAY_MYENGINE_BEZIER_CURVE
#define MYUPLAY_MYENGINE_BEZIER_CURVE

#include <vector>

namespace MyUPlay {

	namespace MyEngine {

		/**
		 * This allows the class to exist in any dimension.
		 *
		 * The template type must support standard math operations.
		 */
		template <typename T>
		class BezierCurve {

			//Should this be optimized to be cubic curves only?
			//This willl require matrix4 and a custom sized matrix class...

			//Notes:
			// http://stackoverflow.com/questions/7942840/bezier-curve-evaluation
			// http://stackoverflow.com/questions/4089443/find-the-tangent-of-a-point-on-a-cubic-bezier-curve-on-an-iphone

			private:
			std::vector<T> points;

			public:

			BezierCurve(){};
			BezierCurve(std::vector<T> points) : points(points){};

			void addPoint(T);
			void removePoint(unsigned index);


		};

	}

}

#endif

