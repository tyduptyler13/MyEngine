#ifndef MYUPLAY_MYENGINE_GLES2MATERIALLIB
#define MYUPLAY_MYENGINE_GLES2MATERIALLIB

#include "MaterialLib.hpp"
#include "GLES2Renderer.hpp"

namespace MyUPlay {
	namespace MyEngine {

		template <>
		std::shared_ptr<IMaterial> createNormalMaterial<GLES2Renderer>();

	}
}

#ifdef NBINDING_MODE

namespace {

	using namespace MyUPlay::MyEngine;

	NBIND_GLOBAL() {

		NBIND_FUNCTION(createNormalMaterial<GLES2Renderer>, "createGLES2NormalMaterial");

	}

}

#endif

#endif
