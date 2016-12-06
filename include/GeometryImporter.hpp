#ifndef MYUPLAY_MYENGINE_GEOMETRYIMPORTER
#define MYUPLAY_MYENGINE_GEOMETRYIMPORTER

#include <string>
#include <functional>

#include "Object3D.hpp"

namespace MyUPlay {
	namespace MyEngine {
		struct GeometryImporter {

			static Object3D<float>* ImportAsset(std::string);
			static void ImportAssetAsync(std::string, std::function<void(Object3D<float>*)>);

		};
	}
}



#endif
