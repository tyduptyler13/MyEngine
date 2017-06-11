#ifndef MYUPLAY_MYENGINE_DEFAULTRENDERER
#define MYUPLAY_MYENGINE_DEFAULTRENDERER

#include "Renderer.hpp"

#include <bgfx/bgfx.h>
#include <bgfx/defines.h>

namespace MyUPlay {
	namespace MyEngine {
		/**
		 * The default renderer can be used for any standard GPU oriented renderer that BGFX supports.
		 *
		 * This class does require a small overload that gives it a way to create and destroy windows/contexts.
		 */
		struct DefaultRenderer : public Renderer<float> {

			DefaultRenderer(bgfx::RendererType type, uint16_t vendorId = BGFX_PCI_ID_NONE);
			~DefaultRenderer();




			virtual void createWindow();

		};
	}
}

#endif
