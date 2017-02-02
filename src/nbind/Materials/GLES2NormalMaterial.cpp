
#include "Materials/GLES2NormalMaterial.hpp"

#include "nbind/nbind.h"

using namespace std;
using namespace MyUPlay::MyEngine;

NBIND_CLASS(GLES2NormalMaterial) {
	inherit(IMaterial);

	construct<>();
	construct<string>();
	construct<const GLES2NormalMaterial&>();
}
