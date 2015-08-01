

#ifndef MyUPlay_MyEngine_Scene
#define MYUPlay_MyEngine_Scene

#include "Object3D.hpp"

namespace MyUPlay {

	namespace MyEngine {


		class Scene : public Object3D {

			private:

			Material* overrideMaterial = NULL;
			Fog* fog = NULL;

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

