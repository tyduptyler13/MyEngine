/*
 * DrawableObject3D.hpp
 *
 *  Created on: Jul 26, 2016
 *      Author: tyler
 */

#ifndef MYUPLAY_MYENGINE_DRAWABLEOBJECT3D
#define MYUPLAY_MYENGINE_DRAWABLEOBJECT3D

#include "Object3D.hpp"
#include "Material.hpp"
#include "Geometry.hpp"

namespace MyUPlay {
	namespace MyEngine {

		template <typename T>
		class DrawableObject3D : public Object3D<T> {

		public:

			DrawableObject3D() : Object3D<T>(DRAWABLE) {}
			virtual ~DrawableObject3D(){}

			Geometry<T> geometry;
			Material<T> material;

			virtual DrawableObject3D& copy(const DrawableObject3D& o) {
				Object3D<T>::copy(o);
				geometry = o.geometry;
				material = o.material;
				return *this;
			}

			DrawableObject3D& operator=(const DrawableObject3D& o){
				return copy(o);
			}

			DrawableObject3D(const DrawableObject3D& o){
				copy(o);
			}

		};

	}
}


#endif /* INCLUDE_DRAWABLEOBJECT3D_HPP_ */
