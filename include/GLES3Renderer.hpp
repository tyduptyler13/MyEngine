#ifndef MYUPLAY_MYENGINE_GLES3RENDERER
#define MYUPLAY_MYENGINE_GLES3RENDERER

#include <vector>

#include "Renderer.hpp"
#include "Texture.hpp"

namespace MyUPlay {

  namespace MyEngine {

    class GLES3Renderer : Renderer<float> {

    public:

      GLES3Renderer();
      ~GLES3Renderer();

      void setScissor(int x, int y, unsigned width, unsigned height) override;
      void setScissorTest(bool enable = true) override;

      void setClearColor(const Color&, float alpha) override;
      Color getClearColor() const override;
      float getClearAlpha() const override;

      void clear(bool color = true, bool depth = true, bool stencil = true) override;

      void clearColor() override;
      void clearDepth() override;
      void clearStencil() override;
      void clearTarget(RenderTarget<T>& target, bool color = true, bool depth = true, bool stencil = true) override;

      bool supportsFloatTextures() const override;
      bool supportsStandardDerivatives() const override;
      bool supportsCompressedTextureS3TC() const override;

      unsigned getMaxAnisotripy() const override;

      void setViewport(int x, int y, unsigned width, unsigned height) override;
      std::tuple<int, int, unsigned, unsigned> getViewport() const override;
      void setDefaultViewport() override;

      void renderBufferImmediate(const Object3D<T>& object, const ShaderProgram& program, const Material<T>& material) override;
      void renderBufferDirect(const Camera<T>& camera, const std::vector<Light<T> >& lights, const Fog<T>& fog, const Material<T>& material, const Object3D<T>& object, const Object3D<T>& objectGroup) override;

      void render(const Scene<T>& scene, const Camera<T>& camera, RenderTarget<T>* renderTarget = NULL, bool forceClear = false) override;

      void setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection) override;
      void setTexture(const Texture& texture, unsigned slot = 0) override;
      void setRenderTarget(RenderTarget& target) override;
      void RenderTarget<T>& getRenderTarget() override;
      void readRenderTargetPixels(RenderTarget<T>& target, int x, int y, unsigned width, unsigned height, void** buffer) override; //TODO Find type for buffer

    private:

      SDL_GLContext* context;

      struct GPUText {
        unsigned id;
        unique_ptr<Texture> texture;

      }

    };

  }

}


#endif
