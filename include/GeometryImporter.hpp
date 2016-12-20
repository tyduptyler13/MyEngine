#ifndef MYUPLAY_MYENGINE_GEOMETRYIMPORTER
#define MYUPLAY_MYENGINE_GEOMETRYIMPORTER

#include <string>
#include <functional>

#include "Object3D.hpp"

namespace MyUPlay {
	namespace MyEngine {
		struct GeometryImporter {

			static Object3D<float>* ImportAsset(std::string);

			static std::shared_ptr<Object3D<float>> ImportSharedAsset(std::string s) {
				return std::shared_ptr<Object3D<float>>(ImportAsset(s));
			}

		};
	}
}

#ifdef NBINDING_MODE

namespace {

	using namespace MyUPlay::MyEngine;

	NBIND_CLASS(GeometryImporter) {

		multimethod(ImportSharedAsset, args(std::string), "ImportAsset");

	}

}

#endif

#endif
