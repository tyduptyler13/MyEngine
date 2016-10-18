#ifndef MYUPLAY_MYENGINE_RENDERTARGET
#define MYUPLAY_MYENGINE_RENDERTARGET

#include <memory>

#include "Math.hpp"
#include "Texture.hpp"

#include "Shader/Attribute.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class GLES2Renderer;

		template <class R> //Renderer
		struct RenderTarget : public Shader::IAttribute<R> {

			Math::UUID uuid = Math::generateUUID();

			unsigned width, height;

			bool depthBuffer = true,
			     stencilBuffer = true;

			std::shared_ptr<RenderTarget<R>> shareDepthFrom = NULL;

			RenderTarget(unsigned width, unsigned height, bool depthBuffer = true, bool stencilBuffer = true)
			: width(width), height(height), depthBuffer(depthBuffer), stencilBuffer(stencilBuffer) {}

			//We also need to clean up the depth buffer if it is deleted.
			~RenderTarget();

			//Most framebuffers don't
			void push() override;
			//Bind the framebuffer in the context.
			void bind() override;

		};

		template <>
		struct RenderTarget<GLES2Renderer> : public Shader::IAttribute<GLES2Renderer> {
			Math::UUID uuid = Math::generateUUID();
			GLuint id; //How to identify the buffer.

			unsigned width, height;

			bool depthBuffer = true,
				 stencilBuffer = true;

			std::shared_ptr<RenderTarget<GLES2Renderer>> shareDepthFrom = NULL;

			RenderTarget(unsigned width, unsigned height, bool depthBuffer = true, bool stencilBuffer = true);

			//We also need to clean up the depth buffer if it is deleted.
			~RenderTarget();

			void push() override {} //Nothing to push
			//Bind the framebuffer in the context.
			void bind() override;
		};

	}

}

#endif
