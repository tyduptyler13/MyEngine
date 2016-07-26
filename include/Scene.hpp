#ifndef MYUPLAY_MYENGINE_SCENE
#define MYUPLAY_MYENGINE_SCENE

#include "Object3D.hpp"
#include "DrawableObject3D.hpp"
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
			std::forward_list<std::weak_ptr<DrawableObject3D<T> > > opaqueObjects, transparentObjects;

			std::shared_ptr<Material<T> > overrideMaterial;
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

			Object3D<T>& add(std::shared_ptr<Object3D<T>> self, std::shared_ptr<Object3D<T>> o) {
				switch (o->type){
				case ObjectType::LIGHT:
					lights.push_front(o);
					break;
				case DRAWABLE: //TODO Add drawable types.
					std::shared_ptr<DrawableObject3D<T>> co = std::dynamic_pointer_cast<DrawableObject3D<T>>(o);
					if (co->material.transparent){
						transparentObjects.push_front(co);
					} else {
						opaqueObjects.push_front(co);
					}

					break;

				default:
					break;

				}

				return Object3D<T>::add(self, o);

			}

			Object3D<T>& remove(std::shared_ptr<Object3D<T>> self, std::shared_ptr<Object3D<T>> o) {
				switch (o->type){
				case ObjectType::LIGHT:

					auto pos = std::find_if(lights.begin(), lights.end(), COMP_WEAK_SHARED(o, Light<T>));
					if (pos != lights.end()){
						lights.erase(pos);
					}

					break;
				case DRAWABLE:
					std::shared_ptr<DrawableObject3D<T>> co = std::dynamic_pointer_cast<DrawableObject3D<T>>(o);
					if (co->material.transparent){
						auto pos = std::find_if(transparentObjects.begin(), transparentObjects.end(), COMP_WEAK_SHARED(co, DrawableObject3D<T>));
						if (pos != transparentObjects.end()){
							transparentObjects.erase(co);
						}
					} else {
						auto pos = std::find_if(opaqueObjects.begin(), opaqueObjects.end(), COMP_WEAK_SHARED(co, DrawableObject3D<T>));
						if (pos != opaqueObjects.end()){
							opaqueObjects.erase(co);
						}
					}

					break;

				default:
					break;

				}

				return Object3D<T>::remove(self, o);

			}

		};

	}

}

#endif
