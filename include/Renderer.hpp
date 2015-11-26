#ifndef MYUPLAY_MYENGINE_RENDERER
#define MYUPLAY_MYENGINE_RENDERER

#include <memory>

#include "Color.hpp"
#include "Constants.hpp"
#include "RenderPlugin.hpp"
#include "RenderTarget.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Renderer {

		public:
			bool autoClear = true,
			     autoClearColor = true,
			     autoClearDepth = true,
			     autoClearStencil = true,
			     sortObjects = true;

			float gammaFactor = 2.0;
			bool gammaInput = false,
			     gammaOutput = false;

			bool shadowMapEnabled = false;
			short shadowMapType = PCFShadowMap,
			      shadowMapCullFace = CullFaceFront;
			bool shadowMapDebug = false,
			     shadowMapCascade = false;

			unsigned maxMorphTargets = 8,
				 maxMorphNormals = 4;

			bool autoScaleCubeMaps = true;

			struct {
				struct {
					unsigned programs = 0,
						 geometries = 0,
						 textures = 0;
				} memory;
				struct {
					unsigned calls = 0,
						 vertices = 0,
						 faces = 0,
						 points = 0;
				} renderer;
			} info;

			std::unique_ptr<RenderPlugin> shadowMapPlugin;

			virtual bool supportsVertexTextures() const = 0;
			virtual void setSize(unsigned width, unsigned height) = 0;
			virtual void setViewPort(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
			virtual void setScissor(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
			virtual void enableScissorTest(bool enable = true) = 0;

			virtual void setClearColor(const Color&, float alpha) = 0;
			virtual Color getClearColor() const = 0;
			virtual float getClearAlpha() const = 0;

			virtual void clear(bool color = true, bool depth = true, bool stencil = true) = 0;

			virtual void clearColor() = 0;
			virtual void clearDepth() = 0;
			virtual void clearStencil() = 0;
			virtual void clearTarget(RenderTarget& target, bool color = true, bool depth = true, bool stencil = true) = 0;
			
			virtual bool supportsFloatTextures() const = 0;
			virtual bool supportsStandardDerivatives() const = 0;
			virtual bool supportsCompressedTextureS3TC() const = 0;

			virtual unsigned getMaxAnisotripy() const = 0;

		};

	}

}

#endif

