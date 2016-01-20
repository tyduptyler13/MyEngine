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

		private:
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

		private: //This section will likely be removed. Serves little point here. 
			//Not meant to be called by people.
			
			//GL state manager functions
			virtual void init() = 0;

			virtual void initAttributes(){
				newAttributes.fill(0);
			}

			virtual void enableAttribute(uint8_t attribute) = 0;

			virtual void enableAttributeAndDivisor(uint8_t attribute, uint8_t meshPerAttribute) = 0;//Extension?

			virtual void disableUnusedAttributes() = 0;

			virtual void setBlending(short blending, short blendEquation, short blendSrc, short blendDst, short blendEquationAlpha, short blendSrcAlpha, short blendDstAlpha) = 0;

			virtual void setDepthFunction(short) = 0;
			virtual void setDepthTest(bool) = 0;
			virtual void setDepthWrite(bool) = 0;

			virtual void setColorWrite(bool) = 0;

			virtual void setFlipSided(bool) = 0;

			virtual void setLineWidth(unsigned short) = 0;

			virtual void setPolygonOffset(bool polygonOffset, T factor, T units) = 0;

			virtual void setScissorTest(bool) = 0;

			virtual void activeTexture() = 0;
			virtual void activeTexture(unsigned short slot) = 0;

			virtual void reset() = 0;

		public:

			virtual float getPixelRatio() const = 0;
			virtual void setPixelRatio(float) = 0;

			virtual std::tuple<unsigned, unsigned> getSize() = 0;
			virtual void setSize(unsigned width, unsigned height) = 0;

			virtual void setViewport(int x, int y, unsigned width, unsigned height) = 0;
			virtual std::tuple<int, int, unsigned, unsigned> getViewport() = 0;

		};

	}

}

#endif

