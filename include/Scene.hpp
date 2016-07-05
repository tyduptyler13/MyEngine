#ifndef MyUPlay_MyEngine_Scene
#define MyUPlay_MyEngine_Scene

#include "Object3D.hpp"
#include "Material.hpp"
#include "Fog.hpp"

#include <memory>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Scene : public Object3D<T> {

			private:

			std::vector<std::weak_ptr<Light<T> > > lights;
			std::vector<std::weak_ptr<Object3D<T> > > opaqueObjects, transparentObjects;

			public:

			std::shared_ptr<Material<T> > overrideMaterial = NULL;
			std::shared_ptr<Fog<T> > fog = NULL;

			bool autoUpdate = true;

			Scene() : Object3D<T>() {}

			Scene(const Scene& scene) : Object3D<T>(scene) {
				overrideMaterial = scene.overrideMaterial;
				fog = scene.fog;
				autoUpdate = scene.autoUpdate;
			}

			virtual Scene& add(Object3D&) override;
			virtual Scene& remove(Object3D&) override;

		};

	}

}

#endif

