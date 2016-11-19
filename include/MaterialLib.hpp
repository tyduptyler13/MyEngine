#ifndef MYUPLAY_MYENGINE_MATERIALLIB
#define MYUPLAY_MYENGINE_MATERIALLIB

#include "Material.hpp"

namespace MyUPlay {
	namespace MyEngine {

		template <class R>
		IMaterial* createNormalMaterial(){
			static_assert(true, "Material not implemented for this renderer or missing header!");
			return nullptr;
		}

	}
}

#endif
