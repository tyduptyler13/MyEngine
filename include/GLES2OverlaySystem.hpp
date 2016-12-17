#ifndef MYUPLAY_MYENGINE_GLES2OVERLAYSYSTEM
#define MYUPLAY_MYENGINE_GLES2OVERLAYSYSTEM

#include "GLES2Renderer.hpp"
#include "OverlaySystem.hpp"
#include "Math.hpp"
#include "Scene.hpp"
#include "Shader/GLES2Shader.hpp"

#include <GLES2/gl2.h>

namespace MyUPlay {

	namespace MyEngine {

		template<>
		struct OverlaySystem<GLES2Renderer> {

			Math::UUID uuid = Math::generateUUID();

			GLuint tex = 0;

			OverlaySystem(){
				glGenTextures(1, &tex);



			}
			~OverlaySystem(){
				glDeleteTextures(1,&tex);
			}

			void updatePixelData(unsigned char* data, unsigned width, unsigned height) {

				glBindTexture(GL_TEXTURE_2D, tex);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			}

			void attach(GLES2Renderer& renderer) {

				renderer.registerPreRenderPlugin(uuid, [this](Scene<float>& scene, Camera<float>* camera){

				});

			}

		private:



		};

		typedef OverlaySystem<GLES2Renderer> GLES2OverlaySystem;

	}

}

#endif
