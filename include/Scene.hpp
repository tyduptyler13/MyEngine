#ifndef MYUPLAY_MYENGINE_SCENE
#define MYUPLAY_MYENGINE_SCENE

#include "Object3D.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Fog.hpp"
#include "Light.hpp"

#include <memory>
#include <forward_list>


/**
 * This function is a macro for creating a lambda compare for weak_ptr vs shared_ptr
 * (Assuming the container has the weak_ptrs)
 * x is the input to the lambda
 * t is the type inside the weak_ptr
 */
#define COMP_WEAK_SHARED(x, t) ( \
		[&x](std::weak_ptr<t>& a){ \
	auto s = a.lock(); \
	if (s){ \
		return s == x; \
	} \
	return false; \
})

namespace MyUPlay {

	namespace MyEngine {

		template <typename T = float>
		class Scene : public Object3D<T> {

		public:
			std::forward_list<std::weak_ptr<Light<T> > > lights;
			std::forward_list<std::weak_ptr<Mesh<T> > > opaqueObjects, transparentObjects;

			std::shared_ptr<IMaterial> overrideMaterial;
			std::shared_ptr<Fog<T> > fog;

			bool autoUpdate = true;

			Scene() : Object3D<T>(SCENE) {}

			Scene(const Scene& scene) : Object3D<T>(scene) {
				overrideMaterial = scene.overrideMaterial;
				fog = scene.fog;
				autoUpdate = scene.autoUpdate;

				lights = scene.lights;
				opaqueObjects = scene.opaqueObjects;
				transparentObjects = scene.transparentObjects;
			}

			Scene& copy(const Scene& s){
				Object3D<T>::copy(s);
				overrideMaterial = s->Y;
				fog = s.fog;
				autoUpdate = s.autoUpdate;

				lights = s.lights;
				opaqueObjects = s.opaqueObjects;
				transparentObjects = s.transparentObjects;
				return *this;
			}

			Scene& operator=(const Scene& s){
				return copy(s);
			}

			//Overloaded version. Explicitly adds lights to our special list.
			Scene& add(std::shared_ptr<Light<T>> l){
				lights.push_front(l);
				this->add(std::static_pointer_cast<Object3D<T>>(l));
				return *this;
			}

			//Overloaded version. Explicity handles drawable objects.
			Scene& add(std::shared_ptr<Mesh<T>> m){
				if (m->material->transparent){
					transparentObjects.push_front(m);
				} else {
					opaqueObjects.push_front(m);
				}
				this->add(std::static_pointer_cast<Object3D<T>>(m));
				return *this;
			}

			Scene& add(std::shared_ptr<Object3D<T>> o) {
				Object3D<T>::add(o);
				return *this;
			}

			Scene& remove(std::shared_ptr<Light<T>> l){
				auto pos = std::find_if(lights.begin(), lights.end(), COMP_WEAK_SHARED(l, Light<T>));
				if (pos != lights.end()){
					lights.erase(pos);
				}
				this->remove(std::static_pointer_cast<Object3D<T>>(l));
				return *this;
			}

			Scene& remove(std::shared_ptr<Mesh<T>> m){
				if (m->material.transparent){
					auto pos = std::find_if(transparentObjects.begin(), transparentObjects.end(), COMP_WEAK_SHARED(m, Mesh<T>));
					if (pos != transparentObjects.end()){
						transparentObjects.erase(pos);
					}
				} else {
					auto pos = std::find_if(opaqueObjects.begin(), opaqueObjects.end(), COMP_WEAK_SHARED(m, Mesh<T>));
					if (pos != opaqueObjects.end()){
						opaqueObjects.erase(pos);
					}
				}

				this->remove(std::static_pointer_cast<Object3D<T>>(m));
				return *this;
			}

		};

	}

}

#endif
