#include <string>

#include "Object3D.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Raycaster.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Mesh : public Object3D<T> {

		public:

			Geometry<T> geometry;
			Material<T> Material;

			void updateMorphTargets();
			unsigned getMorphTargetIndexByName(const std::string& s) const;
			void raycast(const Raycaster&, std::vector<Intersection>&);

		};

	}

}

