#ifndef MYUPLAY_MYENGINE_GLES2RENDERTARGET
#define MYUPLAY_MYENGINE_GLES2RENDERTARGET

#include "RenderTarget.hpp"

namespace MyUPlay {
	namespace MyEngine {

		struct GLES2RenderTarget : public IRenderTarget {
			unsigned buf; //How to identify the buffer.
			unsigned tex;
			unsigned rendBuf = 0; //Might go unused

			GLES2RenderTarget(unsigned width, unsigned height, bool depthBuffer = true, bool stencilBuffer = true, bool alpha = false);

			//We also need to clean up the depth buffer if it is deleted.
			~GLES2RenderTarget();

			//Bind the framebuffer in the context.
			void bind();
		};
	}
}

#endif
