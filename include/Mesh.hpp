#include <string>

#include "Object3D.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Raycaster.hpp"

namespace MyUPlay {

	namespace MyEngine {

		#ifndef GeometryDefined
		template <typename T>
		class Geometry;
		#endif

		template <typename T>
		class Mesh : public Object3D<T> {

		public:

			Geometry<T> geometry;
			Material<T> Material;

			void updateMorphTargets();
			unsigned getMorphTargetIndexByName(const std::string& s) const;
			void raycast(const Raycaster<T>&, std::vector<Intersection<T> >&);

		};

	}

}

