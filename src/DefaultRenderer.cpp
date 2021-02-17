#include "DefaultRenderer.hpp"

using namespace std;
using namespace MyEngine;

DefaultRenderer::DefaultRenderer(bgfx::RendererType::Enum type, uint16_t vendorId) {

	bgfx::Init init;
	init.type = type;
	init.resolution.height = this->windowHeight;
	init.resolution.width = this->windowWidth;
	init.debug = true;
	bgfx::init(init);

}

DefaultRenderer::~DefaultRenderer() {
	bgfx::shutdown();
}
