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
			typedef typename Object3D<T>::EventType EventType;

			template <typename T2>
			static void removeFirst(std::vector<T2>& v, T2 t) {
				auto it = std::find(v.begin(), v.end(), t);
				//Swap and pop. Fast and cheap.
				std::swap(*it, v.back());
				v.pop_back();
			}

			void handleEvents(Object3D<T>* o, EventType e){
				switch(e){
				case EventType::ADD: {
					o->eventHandlers.push_back(&handleEvents);
					switch(o->type){
					case ObjectType::MESH: {
						Mesh<T>* m = dynamic_cast<Mesh<T>*>(o);
						if (m->material->transparent){
							transparentObjects.push_back(m);
						} else {
							opaqueObjects.push_back(m);
						}
					}
					break;
					case ObjectType::LIGHT:
						lights.push_back(dynamic_cast<Light<T>*>(o));
					}

					break;
				}
				case EventType::DELETE:
					switch(o->type){
					case ObjectType::MESH: {
						Mesh<T>* m = dynamic_cast<Mesh<T>*>(o);
						if (m->material->transparent){
							removeFirst(transparentObjects, m);
						} else {
							removeFirst(opaqueObjects, m);
						}
					}
					break;
					case ObjectType::LIGHT:
						removeFirst(lights, o);
					}
				}
			}

		public:
			std::forward_list<Light<T>*> lights;
			std::forward_list<Mesh<T>*> opaqueObjects, transparentObjects;

			std::unique_ptr<IMaterial> overrideMaterial;
			std::unique_ptr<Fog<T> > fog;

			bool autoUpdate = true;

			Scene() : Object3D<T>(ObjectType::SCENE) {}

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
			Scene& add(Light<T>* l){
				lights.push_front(l);
				this->add(static_cast<Object3D<T>*>(l));
				return *this;
			}

			//Overloaded version. Explicity handles drawable objects.
			Scene& add(Mesh<T>* m){
				if (m->material->transparent){
					transparentObjects.push_front(m);
				} else {
					opaqueObjects.push_front(m);
				}
				this->add(static_cast<Object3D<T>*>(m));
				return *this;
			}

			Scene& add(Object3D<T>* o) {
				Object3D<T>::add(o);
				o->eventHandlers.push_back(&handleEvents);
				return *this;
			}

			Scene& remove(Light<T>* l){
				auto pos = std::find(lights.begin(), lights.end(), l);
				if (pos != lights.end()){
					lights.erase(pos);
				}
				this->remove(static_cast<Object3D<T>*>(l));
				return *this;
			}

			Scene& remove(Mesh<T>* m){
				if (m->material.transparent){
					auto pos = std::find(transparentObjects.begin(), transparentObjects.end(), m);
					if (pos != transparentObjects.end()){
						transparentObjects.erase(pos);
					}
				} else {
					auto pos = std::find(opaqueObjects.begin(), opaqueObjects.end(), m);
					if (pos != opaqueObjects.end()){
						opaqueObjects.erase(pos);
					}
				}

				this->remove(static_cast<Object3D<T>*>(m));
				return *this;
			}

		};

	}

}

#endif
