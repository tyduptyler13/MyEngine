
#include "Material.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;

NBIND_CLASS(IMaterial) {

	construct<>();
	construct<std::string>();
	construct<const IMaterial&>();

}

