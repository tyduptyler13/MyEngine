#ifndef MYUPLAY_MYENGINE_RENDERTARGET
#define MYUPLAY_MYENGINE_RENDERTARGET

#include <memory>

#include "Math.hpp"
#include "Texture.hpp"

#include "Shader/Attribute.hpp"

namespace MyUPlay {

	namespace MyEngine {

		struct IRenderTarget : public Shader::IAttribute {

			Math::UUID uuid = Math::generateUUID();

			unsigned width, height;

			bool depthBuffer = true,
			     stencilBuffer = true;

			std::shared_ptr<IRenderTarget> shareDepthFrom;

			//We also need to clean up the depth buffer if it is deleted.
			virtual ~IRenderTarget(){};

			//Bind the framebuffer in the context.
			virtual void bind() = 0;

		};

	}

}

#endif
