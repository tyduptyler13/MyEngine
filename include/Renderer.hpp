#ifndef MYUPLAY_MYENGINE_RENDERER
#define MYUPLAY_MYENGINE_RENDERER

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <tuple>

#include "Color.hpp"
#include "Constants.hpp"
#include "RenderPlugin.hpp"
#include "RenderTarget.hpp"
#include "Light.hpp"
#include "Object3D.hpp"
#include "Camera.hpp"
#include "Material.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T = float>
		class Renderer {

		protected:
			std::array<uint8_t, 16> newAttributes;
			std::array<uint8_t, 16> enabledAttributes;
			std::array<uint8_t, 16> attributeDivisors;

			short currentBlending;
			short currentBlendEquation;
			short currentBlendSrc;
			short currentBlendDst;
			short currentBlendEquationAlpha;
			short currentBlendSrcAlpha;
			short currentBlendDstAlpha;

			short currentDepthFunc;
			short currentDepthWrite;

			short currentColorWrite;

			short currentFlipSided;

			short currentLineWidth;

			short currentPolygonOffsetFactor;
			short currentPolygonOffsetUnits;

			const unsigned short maxTextures;

			short currentTextureSlot;
			//currentBoundTextures;

			Color mClearColor;
			float mClearAlpha;

			unsigned width, height;

			float mPixelRatio;

			int mViewportX = 0,
			    mViewportY = 0;
			unsigned mViewportWidth,
				 mViewportHeight;

			bool alpha = false,
			     depth = true,
			     stencil = true,
			     antialias = false,
			     premultipliedAlpha = true,
			     preserveDrawingBuffer = false;

			std::vector<Light<T>*> lights;
			std::vector<Object3D<T>*> opaqueObjects, transparentObjects;
			int opaqueObjectsLastIndex = -1, transparentObjectsLastIndex = -1;

		public:

			bool autoClear = true,
			     autoClearColor = true,
			     autoClearDepth = true,
			     autoClearStencil = true,
			     sortObjects = true;

			T gammaFactor = 2.0;
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
			virtual void setScissor(int x, int y, unsigned width, unsigned height) = 0;
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

			virtual float getPixelRatio() const = 0;
			virtual void setPixelRatio(float) = 0;

			virtual std::tuple<unsigned, unsigned> getSize() = 0;
			virtual void setSize(unsigned width, unsigned height) = 0;

			virtual void setViewport(int x, int y, unsigned width, unsigned height) = 0;
			virtual std::tuple<int, int, unsigned, unsigned> getViewport() = 0;

			virtual void renderBufferImmediate(const Object3D<T>& object, const ShaderProgram& program, const Material<T>& material) = 0;
			virtual void renderBufferDirect(const Camera<T>& camera, const std::vector<Light<T> >& lights, const Fog<T>& fog, const Material<T>& material, const Object3D<T>& object, const Object3D<T>& objectGroup) = 0;

			virtual void render(const Scene<T>& scene, const Camera<T>& camera, RenderTarget* renderTarget = NULL, bool forceClear = false) = 0;

			virtual void setFaceCulling(short cullFace, short frontFaceDirection) = 0;
			virtual void setTexture(const Texture<T>& texture, unsigned slot = 0);
			virtual void setRenderTarget(RenderTarget& target) = 0;
			virtual void readRenderTargetPixels(RenderTarget& target, int x, int y, unsigned width, unsigned height, void** buffer) = 0; //TODO Find type for buffer

		};

	}

}

#endif

