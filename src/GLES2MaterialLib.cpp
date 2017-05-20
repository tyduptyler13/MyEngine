
#include "GLES2MaterialLib.hpp"
#include "Log.hpp"
#include "Materials/GLES2NormalMaterial.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;



shared_ptr<IMaterial> MyUPlay::MyEngine::GLES2CreateNormalMaterial() {
	auto console = spdlog::get("MyEngine");
	console->warn("This function is deprecated and will be removed soon. See GLES2NormalMaterial class.");
	return make_shared<GLES2NormalMaterial>();
}
