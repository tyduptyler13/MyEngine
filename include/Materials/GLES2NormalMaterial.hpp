#ifndef MYUPLAY_MYENGINE_GLES2NORMALMATERIAL
#define MYUPLAY_MYENGINE_GLES2NORMALMATERIAL

#include "Material.hpp"
#include "Shader/GLES2Shader.hpp"

#include <memory>

namespace MyUPlay {
	namespace MyEngine {

		class GLES2NormalMaterial : public IMaterial {

		public:

			//No special attributes. All inheritted from IMaterial

			GLES2NormalMaterial() : GLES2NormalMaterial("NormalMaterial") {}

			GLES2NormalMaterial(std::string name) : IMaterial(name) {
				this->shader = std::make_shared<Shader::GLES2ForwardShader>(); //The default shader is a normal shader.
			}

			GLES2NormalMaterial(const GLES2NormalMaterial& mat) : IMaterial(mat) {
				this->shader = mat.shader;
			}

		};

	}
}

#endif
