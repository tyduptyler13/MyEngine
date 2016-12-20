#ifndef MYUPLAY_MYENGINE_MATERIALLIB
#define MYUPLAY_MYENGINE_MATERIALLIB

#include "Material.hpp"

#include <memory>
#include <cassert>

namespace MyUPlay {
	namespace MyEngine {

		template <class R>
		std::shared_ptr<IMaterial> createNormalMaterial(){
			static_assert(true, "Material not implemented for this renderer or missing header!");
			assert(false); //YOU SHOULD NEVER REACH THIS!
			return nullptr;
		}

	}
}

#endif
