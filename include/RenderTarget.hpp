#pragma once

#include <memory>

#include "Math.hpp"
#include "Texture.hpp"


namespace MyEngine {

	struct IRenderTarget {

		Math::UUID uuid = Math::generateUUID();

		unsigned width, height;

		bool depthBuffer = true,
				stencilBuffer = true;

		//We also need to clean up the depth buffer if it is deleted.
		virtual ~IRenderTarget() {};

		//Bind the framebuffer in the context.
		virtual void bind() = 0;

	protected:

		IRenderTarget(unsigned width, unsigned height, bool depthBuffer, bool stencilBuffer)
				: width(width), height(height), depthBuffer(depthBuffer), stencilBuffer(stencilBuffer) {}

	};

}
