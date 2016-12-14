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

#ifdef NBINDING_MODE

namespace {

	using namespace MyUPlay::MyEngine;

	std::shared_ptr<IMaterial> createGLES2NormalMaterial() {
		return std::shared_ptr<IMaterial>(createNormalMaterial<GLES2Renderer>());
	}

	NBIND_GLOBAL() {

		function(createGLES2NormalMaterial);

	}

}

#endif

#endif
