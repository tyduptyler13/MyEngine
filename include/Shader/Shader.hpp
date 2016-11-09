
#ifndef MYUPLAY_MYENGINE_SHADER
#define MYUPLAY_MYENGINE_SHADER

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {
			struct Shader;
			struct ForwardShader;
			struct DeferredShader;
		}
	}
}

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Light.hpp"

#include "Shader/ShaderUtil.hpp"

struct MyUPlay::MyEngine::Shader::Shader {

	virtual ~Shader(){}

	bool dirty = true;

	virtual void compile() = 0;

	virtual void bind() = 0;

	virtual void prepare(Camera<float>* camera, Mesh<float>* o, const std::vector<Light<float>*>& lights) = 0;

	std::unique_ptr<VertexBase> vertexShaderRoot;

};

struct MyUPlay::MyEngine::Shader::ForwardShader : public virtual Shader {

	std::unique_ptr<FragmentBase> fragmentShaderRoot;

};

struct MyUPlay::MyEngine::Shader::DeferredShader : public virtual Shader {
	std::unique_ptr<DeferredFragment> frag;
	std::unique_ptr<DeferredLightingPass> lighting;
};

#endif
