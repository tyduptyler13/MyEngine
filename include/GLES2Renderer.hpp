#ifndef MYUPLAY_MYENGINE_GLES2RENDERER
#define MYUPLAY_MYENGINE_GLES2RENDERER

#include <string>
#include <unordered_map>

#include "Renderer.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class GLES2Renderer : public Renderer<float> {

		public:

			/**
			 * Antialias should be set to a value 0 (off), or 2-4 for varying samples (on).
			 */
			GLES2Renderer(unsigned antialias = 0);
			~GLES2Renderer();

			void setScissor(int x, int y, unsigned width, unsigned height);
			void setScissorTest(bool enable = true);

			void setClearColor(const Color&, float alpha);
			Color getClearColor() const;
			float getClearAlpha() const;

			void clear(bool color = true, bool depth = true, bool stencil = true);

			void clearColor();
			void clearDepth();
			void clearStencil();
			void clearTarget(std::shared_ptr<IRenderTarget> target, bool color = true, bool depth = true, bool stencil = true);

			unsigned getMaxAnisotripy() const;

			void setViewport(int x, int y, unsigned width, unsigned height);
			std::tuple<int, int, unsigned, unsigned> getViewport() const;
			void setDefaultViewport();

			void renderBufferImmediate(Object3D<float>* object, std::shared_ptr<Shader::Shader> program,IMaterial* material);
			void renderBufferDirect(Camera<float>* camera, std::vector<Light<float>*>& lights, Fog<float>& fog, IMaterial* material, Object3D<float>* object, Object3D<float>* objectGroup);

			void render(Scene<float>& scene, Camera<float>* camera, std::shared_ptr<IRenderTarget> renderTarget = NULL, bool forceClear = false);

			void setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection);
			void setTexture(std::shared_ptr<Texture> texture, unsigned slot = 0);
			void setRenderTarget(std::shared_ptr<IRenderTarget> target);
			std::shared_ptr<IRenderTarget> getRenderTarget();
			void readRenderTargetPixels(std::shared_ptr<IRenderTarget> target, int x, int y, unsigned width, unsigned height, void* buffer); //TODO Find type for buffer

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
