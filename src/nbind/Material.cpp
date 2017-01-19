
#include "Material.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(IMaterial) {

	construct<>();
	construct<std::string>();
	construct<const IMaterial&>();

}

