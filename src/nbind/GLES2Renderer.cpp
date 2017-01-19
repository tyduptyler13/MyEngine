
#include "GLES2Renderer.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(GLES2Renderer) {

	inherit(Renderer<float>);

	construct<unsigned>();

}
