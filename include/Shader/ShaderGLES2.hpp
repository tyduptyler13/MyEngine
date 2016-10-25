#ifndef MYUPLAY_MYENGINE_SHADERGLES2
#define MYUPLAY_MYENGINE_SHADERGLES2

#include "Shader/ShaderNode.hpp"
#include "Shader/Shader.hpp"

#include "GLES2/gl2.h"

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			struct GLES2Shader : public virtual Shader {

				GLuint program = 0;

				void bind() override;
			};

			struct ForwardShaderGLES2 : public ForwardShader, public GLES2Shader {

				~ForwardShaderGLES2();

				void compile() override;

				void render(std::shared_ptr<Camera<float>> camera,
						std::shared_ptr<DrawableObject3D<float>> object,
						const std::vector<Light<float>>& lights) override;

			};

		}
	}
}

#endif
