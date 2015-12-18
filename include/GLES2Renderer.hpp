#ifndef MYUPLAY_MYENGINE_GLES2RENDERER
#define MYUPLAY_MYENGINE_GLES2RENDERER

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengles2.h>

#include "Renderer.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class GLES2Renderer : public Renderer<T> {

		};

	}

}

#endif

