#ifndef MYUPLAY_MYENGINE_GLES2RENDERTARGET
#define MYUPLAY_MYENGINE_GLES2RENDERTARGET

#include <GLES2/gl2.h>

#include "RenderTarget.hpp"

namespace MyUPlay {
	namespace MyEngine {

		struct GLES2RenderTarget : public IRenderTarget {
			GLuint id; //How to identify the buffer.

			GLES2RenderTarget(unsigned width, unsigned height, bool depthBuffer = true, bool stencilBuffer = true)
			: width(width), height(height), depthBuffer(depthBuffer), stencilBuffer(stencilBuffer){
				glGenFramebuffers(1, &id);
			}

			//We also need to clean up the depth buffer if it is deleted.
			~GLES2RenderTarget(){
				glDeleteFramebuffers(1, &id);
			}

			//Bind the framebuffer in the context.
			void bind() {
				glBindFramebuffer(GL_FRAMEBUFFER, id);
			}
		};
	}
}

#endif
