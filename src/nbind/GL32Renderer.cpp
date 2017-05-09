#include "GL32Renderer.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(GL32Renderer) {

	inherit(GLES2Renderer);

	construct<unsigned>();

}
