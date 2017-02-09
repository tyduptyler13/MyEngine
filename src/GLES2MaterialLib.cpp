#include "Log.hpp"
#include "GLES2MaterialLib.hpp"

#include "Materials/GLES2NormalMaterial.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;

Log dlog("GLES2CreateNormalMaterial");

shared_ptr<IMaterial> MyUPlay::MyEngine::GLES2CreateNormalMaterial() {
	dlog.warn("This function is deprecated and will be removed soon. See GLES2NormalMaterial class.");
	return make_shared<GLES2NormalMaterial>();
}
