
#include "GLES2OverlaySystem.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(GLES2OverlaySystem){

	construct<GLES2Renderer&>();

	method(template updatePixelData<nbind::Buffer>, "updatePixelData");

}
