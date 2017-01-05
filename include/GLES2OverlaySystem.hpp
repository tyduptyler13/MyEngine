#ifndef MYUPLAY_MYENGINE_GLES2OVERLAYSYSTEM
#define MYUPLAY_MYENGINE_GLES2OVERLAYSYSTEM

#include "GLES2Renderer.hpp"
#include "Math.hpp"
#include "Scene.hpp"
#include "Shader/GLES2Shader.hpp"

#include <GLES2/gl2.h>

namespace MyUPlay {

	namespace MyEngine {

		struct GLES2OverlaySystem {

			Math::UUID uuid = Math::generateUUID();

			GLES2OverlaySystem(){
				glGenTextures(1, &tex);

				glBindTexture(GL_TEXTURE_2D, tex);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

				glGenBuffers(1, &vertBuf);
				glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
				glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);

				glGenBuffers(1, &indBuf);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6, &indices, GL_STATIC_DRAW);

			}
			~GLES2OverlaySystem(){
				glDeleteTextures(1,&tex);
			}

			template <typename T>
			void updatePixelData(T data, unsigned width, unsigned height) {

				glBindTexture(GL_TEXTURE_2D, tex);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

			}

			void attach(GLES2Renderer& renderer) {

				renderer.registerPostRenderPlugin(uuid, [this, &renderer](Scene<float>& scene, Camera<float>* camera){

					renderer.setDepthTest(false);
					renderer.setDepthWrite(false);

					shader.prepare(this->tex);

					GLint vertPos = shader.getAttribLoc("vertex");

					glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
					glEnableVertexAttribArray(vertPos);
					glVertexAttribPointer(vertPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);

				});

			}

		private:
			GLuint tex = 0;
			GLuint vertBuf, indBuf;

			const std::array<GLfloat, 12> vertices = {{
					-1, -1, 0,
					-1, 1, 0,
					1, 1, 0,
					1, -1, 0
			}};
			const std::array<GLubyte, 6> indices = {{
					0,1,2,
					0,2,3
			}};
			Shader::GLES2FlatShader shader;

		};

	}

}

#ifdef NBINDING_MODE

namespace {

	using namespace MyUPlay::MyEngine;

	NBIND_CLASS(GLES2OverlaySystem){

		construct<>();

		method(template updatePixelData<nbind::Buffer>, "updatePixelData");
		method(attach);

	}

}

#endif

#endif
