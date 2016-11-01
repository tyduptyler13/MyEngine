#ifndef MYUPLAY_MYENGINE_MESH
#define MYUPLAY_MYENGINE_MESH

#include <string>

#include "Object3D.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Raycaster.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		struct Mesh : public Object3D<T> {

			std::unique_ptr<IGeometry<T>> geometry;
			std::unique_ptr<Material<T>> material;

			//void raycast(const Raycaster<T>&, std::vector<Intersection<T> >&); //TODO

		};

	}

}

#endif
