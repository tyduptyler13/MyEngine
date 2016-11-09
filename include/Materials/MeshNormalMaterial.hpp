#ifndef MYUPLAY_MYENGINE_MESHNORMALMATERIAL
#define MYUPLAY_MYENGINE_MESHNORMALMATERIAL

#include "Material.hpp"

namespace MyUPlay {

	namespace MyEngine {

		struct MeshNormalMaterial : public IMaterial {

			bool wireframe = false;
			float wireframeLineWidth = 1;

			bool fog = false,
					lights = false,
					morphTargets = false;

			MeshNormalMaterial() {}

		};

	}

}

#endif
