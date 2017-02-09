#ifndef MYUPLAY_MYENGINE_GLES2OVERLAYSYSTEM
#define MYUPLAY_MYENGINE_GLES2OVERLAYSYSTEM

#include "GLES2Renderer.hpp"
#include "Math.hpp"
#include "Scene.hpp"
#include "Shader/GLES2Shader.hpp"

#include <GLES2/gl2.h>

#include <memory>

namespace MyUPlay {

	namespace MyEngine {

		struct GLES2OverlaySystem {

			Math::UUID uuid = Math::generateUUID();

			GLES2OverlaySystem(GLES2Renderer& r){

				renderer = &r;

				r.glfwFunction([this]{
					glGenTextures(1, &tex);

					glBindTexture(GL_TEXTURE_2D, tex);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					const std::array<GLubyte, 4> blank = {{0,0,0,0}};

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &blank);

					glGenBuffers(1, &vertBuf);
					glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
					glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);

					glGenBuffers(1, &indBuf);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLubyte), &indices, GL_STATIC_DRAW);

				});

				r.registerPostRenderPlugin(uuid, [this, &r](Scene<float>& scene, Camera<float>* camera){

					r.glfwFunction([this, &r]{
						r.setDepthTest(false);
						r.setDepthWrite(false);

						glDisable(GL_CULL_FACE);

						glEnable(GL_BLEND);

						glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
						glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);

						shader.prepare(this->tex);

						GLint vertPos = shader.getAttribLoc("vertex");

						glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
						glEnableVertexAttribArray(vertPos);
						glVertexAttribPointer(vertPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
					});

				});


			}
			~GLES2OverlaySystem(){
				glDeleteTextures(1, &tex);
			}

			template <typename T>
			void updatePixelData(T data, unsigned width, unsigned height) {

				renderer->glfwFunction([&, this]{
					glBindTexture(GL_TEXTURE_2D, tex);

					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					unsigned char* d = data.data();

					for (unsigned i = 0, size = width * height; i < size; ++i) {
						unsigned char tmp = d[i * 4];
						d[i * 4] = d[i * 4 + 2];
						d[i * 4 + 2] = tmp;
					}

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, d);
				});

			}

		private:
			GLuint tex = -1;
			GLuint vertBuf, indBuf;

			const std::array<GLfloat, 8> vertices = {{
					-1, -1,
					-1, 1,
					1, 1,
					1, -1
			}};
			const std::array<GLubyte, 6> indices = {{
					0,2,1,
					0,3,2
			}};
			Shader::GLES2FlatShader shader;
			GLES2Renderer* renderer;

		};

	}

}

#endif
