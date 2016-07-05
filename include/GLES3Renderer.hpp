#ifndef MYUPLAY_MYENGINE_GLES3RENDERER
#define MYUPLAY_MYENGINE_GLES3RENDERER

#include "Renderer.hpp"

namespace MyUPlay {

    namespace MyEngine {

        class GLES3Renderer : Renderer<float> {

            public:

                bool supportsVertexTextures() const override;
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

    			float getPixelRatio() const override;
    			void setPixelRatio(float) override;

    			std::tuple<unsigned, unsigned> getSize() const override;
    			void setSize(unsigned width, unsigned height) override;

    			void setViewport(int x, int y, unsigned width, unsigned height) override;
    			std::tuple<int, int, unsigned, unsigned> getViewport() const override;

    			void renderBufferImmediate(const Object3D<T>& object, const ShaderProgram& program, const Material<T>& material) override;
    			void renderBufferDirect(const Camera<T>& camera, const std::vector<Light<T> >& lights, const Fog<T>& fog, const Material<T>& material, const Object3D<T>& object, const Object3D<T>& objectGroup) override;

    			void render(const Scene<T>& scene, const Camera<T>& camera, RenderTarget<T>* renderTarget = NULL, bool forceClear = false) override;

    			void setFaceCulling(short cullFace, short frontFaceDirection) override;
    			void setTexture(const Texture<T>& texture, unsigned slot override);
    			void setRenderTarget(RenderTarget<T>& target) override;
    			void RenderTarget<T>& getRenderTarget() override;
    			void readRenderTargetPixels(RenderTarget<T>& target, int x, int y, unsigned width, unsigned height, void** buffer) override; //TODO Find type for buffer

        };

    }

}


#endif
