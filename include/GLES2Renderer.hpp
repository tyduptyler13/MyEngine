#ifndef MYUPLAY_MYENGINE_GLES2RENDERER
#define MYUPLAY_MYENGINE_GLES2RENDERER

#include <string>
#include <unordered_map>

#include "Renderer.hpp"
#include "Texture.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class GLES2Renderer : Renderer<GLES2Renderer, float> {

		public:

			/**
			 * Antialias should be set to a value 0 (off), or 2-4 for varying samples (on).
			 */
			GLES2Renderer(unsigned antialias = 0);
			virtual ~GLES2Renderer();

			void setScissor(int x, int y, unsigned width, unsigned height) override;
			void setScissorTest(bool enable = true) override;

			void setClearColor(const Color&, float alpha) override;
			Color getClearColor() const override;
			float getClearAlpha() const override;

			void clear(bool color = true, bool depth = true, bool stencil = true) override;

			void clearColor() override;
			void clearDepth() override;
			void clearStencil() override;
			void clearTarget(RenderTarget& target, bool color = true, bool depth = true, bool stencil = true) override;

			unsigned getMaxAnisotripy() const override;

			void setViewport(int x, int y, unsigned width, unsigned height) override;
			std::tuple<int, int, unsigned, unsigned> getViewport() const override;
			void setDefaultViewport() override;

			void renderBufferImmediate(Object3D<float>& object, std::shared_ptr<Shader::IMasterShaderNode<GLES2Renderer>> program, Material<float>& material) override;
			void renderBufferDirect(Camera<float>& camera, std::vector<Light<float> >& lights, Fog<float>& fog, Material<float>& material, Object3D<float>& object, Object3D<float>& objectGroup) override;

			void render(Scene<float>& scene, Camera<float>& camera, std::shared_ptr<RenderTarget> renderTarget = NULL, bool forceClear = false) override;

			void setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection) override;
			void setTexture(std::shared_ptr<Texture> texture, unsigned slot = 0) override;
			void setRenderTarget(RenderTarget& target) override;
			RenderTarget& getRenderTarget() override;
			void readRenderTargetPixels(RenderTarget& target, int x, int y, unsigned width, unsigned height, void** buffer) override; //TODO Find type for buffer



		private:

			SDL_GLContext context;

			struct GPUTexture { //Wrap textures with some of our own internal variables.
				unsigned id; //Texture handle
				//Unused int slot = -1; //Texture slot, -1 for not bound.
				std::shared_ptr<Texture> texture;
			};

			//Textures are tracked by their guids.
			std::unordered_map<std::string, GPUTexture> textures; //Tracks loaded textures.

		};

	}

}


#endif
