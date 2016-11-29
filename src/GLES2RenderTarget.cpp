#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GLES2RenderTarget.hpp"

using namespace MyUPlay::MyEngine;

GLES2RenderTarget::GLES2RenderTarget(unsigned width, unsigned height, bool depthBuffer, bool stencilBuffer, bool alpha)
: IRenderTarget(width, height, depthBuffer, stencilBuffer) {
	glGenFramebuffers(1, &buf);
	bind();
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0,
			alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	if (depthBuffer || stencilBuffer) {

		glGenRenderbuffers(1, &rendBuf);
		glBindRenderbuffer(GL_RENDERBUFFER, rendBuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
		if (depthBuffer) {
			glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rendBuf);
		}
		if (stencilBuffer){
			glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendBuf);
		}
	}

}

GLES2RenderTarget::~GLES2RenderTarget(){
	glDeleteFramebuffers(1, &buf);
	glDeleteTextures(1, &tex);
	if (rendBuf == 0){
		glDeleteRenderbuffers(1, &rendBuf);
	}
}

void GLES2RenderTarget::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, buf);
}
