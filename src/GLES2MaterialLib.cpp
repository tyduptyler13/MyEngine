#include <memory>

#include "GLES2MaterialLib.hpp"

#include "Shader/ShaderGLES2.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;

template <>
IMaterial* MyUPlay::MyEngine::createNormalMaterial<GLES2Renderer>() {

	IMaterial* mat = new IMaterial("NormalMaterial");

	mat->shader = make_unique<GLES2ForwardShader>(); //The default shader is a normal shader.

	return mat;

}
