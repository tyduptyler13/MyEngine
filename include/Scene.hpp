

#ifndef MyUPlay_MyEngine_Scene
#define MYUPlay_MyEngine_Scene

#include "Object3D.hpp"
#include "Material.hpp"
#include "Fog.hpp"

#include <memory>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Scene : public Object3D<T> {

			private:

			std::unique_ptr<Material> overrideMaterial = NULL;
			std::unique_ptr<Fog> fog = NULL;

			public:

			bool autoUpdate = true;


			Scene() : Object3D() {}

			Scene(const Scene* scene) : Object3D(scene) {
				overrideMaterial = scene->overrideMaterial;
				fog = scene->fog;
				autoUpdate = scene->autoUpdate;
			}

			Scene* clone(){
				return new Scene(this);
			}

			Scene* clone(const Scene* scene){
				return new Scene(scene);
			}

		};

	}

}

#endif

