#include <cassert>

#include "GLES2MaterialLib.hpp"

#include "Materials/GLES2NormalMaterial.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;

shared_ptr<IMaterial> MyUPlay::MyEngine::GLES2CreateNormalMaterial() {
	return make_shared<GLES2NormalMaterial>();
}
