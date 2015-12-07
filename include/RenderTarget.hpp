#ifndef MYUPLAY_MYENGINE_RENDERTARGET
#define MYUPLAY_MYENGINE_RENDERTARGET

#include <memory>

#include "Math.hpp"
#include "Texture.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class RenderTarget {

		public:
			Math::UUID uuid = Math::generateUUID();
			
			unsigned width, height;

			Texture texture;

			bool depthBuffer = true,
			     stencilBuffer = true;

			std::shared_ptr<RenderTarget> shareDepthFrom = NULL;

		};

	}

}

#endif
