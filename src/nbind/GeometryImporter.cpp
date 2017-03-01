
#include "GeometryImporter.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

NBIND_CLASS(GeometryImporter) {

	multimethod(ImportSharedAsset, args(std::string, bool, std::string), "ImportMemoryAsset");
	multimethod(ImportSharedAsset, args(std::string), "ImportAsset");

}
