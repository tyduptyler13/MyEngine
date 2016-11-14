#ifndef MYUPLAY_MYENGINE_GLES2MATERIALLIB
#define MYUPLAY_MYENGINE_GLES2MATERIALLIB

#include "MaterialLib.hpp"
#include "GLES2Renderer.hpp"

namespace MyUPlay {
	namespace MyEngine {

		template <>
		IMaterial* createNormalMaterial<GLES2Renderer>();

	}
}

#endif
