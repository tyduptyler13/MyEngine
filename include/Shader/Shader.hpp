
#ifndef MYUPLAY_MYENGINE_SHADER
#define MYUPLAY_MYENGINE_SHADER

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Light.hpp"

#include "Shader/ShaderUtil.hpp"

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			struct Shader {

				virtual ~Shader(){}

				bool dirty = true;

				virtual void compile() = 0;

				virtual void bind() = 0;

				virtual void render(std::shared_ptr<Camera<float>> camera, std::shared_ptr<Mesh<float>> o, const std::vector<Light<float>>& lights) = 0;

				std::unique_ptr<VertexBase> vertexShaderRoot;

			};

			struct ForwardShader : public virtual Shader {

				std::unique_ptr<FragmentBase> fragmentShaderRoot;

			};

			struct DeferredShader : public virtual Shader {
				std::unique_ptr<DeferredFragment> frag;
				std::unique_ptr<DeferredLightingPass> lighting;
			};
		}
	}
}

#endif
