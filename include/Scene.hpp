#ifndef MYUPLAY_MYENGINE_SCENE
#define MYUPLAY_MYENGINE_SCENE

#include "Object3D.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Fog.hpp"
#include "Light.hpp"

#include <memory>
#include <forward_list>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T = float>
		class Scene : public Object3D<T> {
		private:

			typedef typename Object3D<T>::ObjectType ObjectType;

		public:
			std::shared_ptr<IMaterial> overrideMaterial;
			std::unique_ptr<Fog<T> > fog;

			bool autoUpdate = true;

			Scene() : Object3D<T>(ObjectType::SCENE) {}

			Scene(const Scene& scene) : Object3D<T>(scene) {
				overrideMaterial = scene.overrideMaterial;
				//fog = scene.fog; FIXME
				autoUpdate = scene.autoUpdate;
			}

			Scene& copy(const Scene& s){
				Object3D<T>::copy(s);
				overrideMaterial = s.overrideMaterial;
				//fog = s.fog; FIXME
				autoUpdate = s.autoUpdate;
				return *this;
			}

			Scene& operator=(const Scene& s){
				return copy(s);
			}

			bool getAutoUpdate() const {
				return autoUpdate;
			}

			void setAutoUpdate(bool b) {
				autoUpdate = b;
			}

		};

	}

}

#endif
