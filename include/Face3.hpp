#ifndef MYUPLAY_MYENGINE_FACE3
#define MYUPLAY_MYENGINE_FACE3

#include <memory>
#include <array>

#include "Vector3.hpp"
#include "Color.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Face3 {

		public:

			Vector3<T> a, b, c;
			Vector3<T> normal;
			std::unique_ptr<std::array<Vector3<T>, 3> > vertexNormals = NULL;

			Color color;
			std::unique_ptr<std::array<Color, 3> > vertexColors = NULL;

			unsigned materialIndex = 0;

			Face3(){}
			Face3(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, Vector3<T> normal = Vector3<T>(), Color color = Color(), unsigned materialIndex = 0)
				: a(a), b(b), c(c), normal(normal), color(color), materialIndex(materialIndex) {}
			Face3(const Face3& face){
				copy(face);
			}

			Face3& copy(const Face3& f) {

				a = f.a;
				b = f.b;
				c = f.c;

				normal = f.normal;
				color = f.color;

				materialIndex = f.materialIndex;

				if (f.vertexNormals != NULL){
					vertexNormals = new std::array<Vector3<T>, 3>();
					for (unsigned i = 0; i < 3; ++i){
						vertexNormals.get()[i] = f.vertexNormals[i];
					}
				}

				if (f.vertexColors != NULL){
					vertexColors = new std::array<Color, 3>();
					for (unsigned i = 0; i < 3; ++i){
						vertexColors.get()[i] = f.vertexColors[i];
					}
				}

				return *this;

			}

			Face3& operator=(const Face3& f){
				return copy(f);
			}

		};

	}

}

#endif
