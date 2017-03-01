#ifndef MYUPLAY_MYENGINE_GEOMETRYIMPORTER
#define MYUPLAY_MYENGINE_GEOMETRYIMPORTER

#include <string>
#include <functional>

#include "Object3D.hpp"

namespace MyUPlay {
	namespace MyEngine {
		struct GeometryImporter {

			/**
			 * Geometry importer now requests up to three parameters.
			 *
			 * Default mode is to simply pass a filename relative to execution.
			 *
			 * Advanced mode allows you to pass raw files in the first parameter, set the second to true, and optionally
			 * inform the importer what extension the file used.
			 */
			static Object3D<float>* ImportAsset(std::string, bool raw = false, std::string ext = "");

			static std::shared_ptr<Object3D<float>> ImportSharedAsset(std::string s, bool raw, std::string ext = "") {
				return std::shared_ptr<Object3D<float>>(ImportAsset(s, raw, ext));
			}

			static std::shared_ptr<Object3D<float>> ImportSharedAsset(std::string s) {
				return std::shared_ptr<Object3D<float>>(ImportAsset(s));
			}

		};
	}
}

#endif
