#ifndef MYUPLAY_MYENGINE_RENDERER
#define MYUPLAY_MYENGINE_RENDERER

#include "Color.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Renderer {

		public:
			bool autoClear,
			     autoClearColor,
			     autoClearDepth,
			     autoClearStencil,
			     sortObjects,
			     gammaInput,
			     shadowMapEnabled,
			     shadowMapType,
			     shadowMapCullFace,
			     shadowMapDebug,
			     shadowMapCascade;

			unsigned maxMorphTargets,
				 maxMorphNormals;

			bool autoScaleCubeMaps;

			struct {
				struct {
					unsigned programs,
						 geometries,
						 textures;
				} memory;
				struct {
					unsigned calls,
						 vertices,
						 faces,
						 points;
				} renderer;
			} info;

			RenderPlugin* shadowMapPlugin;

			virtual bool supportsVertexTextures() = 0;
			virtual void setSize(unsigned width, unsigned height) = 0;
			virtual void setViewPort(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
			virtual void setScissor(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
			virtual void enableScissorTest(bool enable = true) = 0;
			
			virtual void setClearColor(Color, float alpha) = 0;
			virtual Color getClearColor() = 0;
			virtual float getClearAlpha() = 0;

			virtual void clear(bool color = true, bool depth = true, bool stencil = true) = 0;

			//TODO


		};

	}

}

#endif

