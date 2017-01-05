#include <memory>
#include <cassert>

#include "GLES2MaterialLib.hpp"

#include "../include/Shader/GLES2Shader.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;

template <>
std::shared_ptr<IMaterial> MyUPlay::MyEngine::createNormalMaterial<GLES2Renderer>() {

	std::shared_ptr<IMaterial> mat = make_shared<IMaterial>("NormalMaterial");

	//TODO We can cache shaders IF we know that the shader exists in the same context.

	mat->shader = make_shared<GLES2ForwardShader>(); //The default shader is a normal shader.

	assert(mat != nullptr);

	return mat;

}
