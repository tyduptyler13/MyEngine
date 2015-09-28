#ifndef MYUPLAY_MYENGINE_OPENGLRENDERER
#define MYUPLAY_MYENGINE_OPENGLRENDERER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Color.hpp"
#include "Light.hpp"
#include "Object3D.hpp"
#include "Frustrum.hpp"
#include "Matrix4.hpp"
#include "Matrix3.hpp"
#include "Vector3.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class OpenGLRenerer{

		private:

			unsigned width, height;
			bool alpha,
			     depth,
			     stencil,
			     antialias,
			     premultipliedAlpha,
			     preserveDrawingBuffer;

			Color clearColor(0x000000);
			auto clearAlpha = 0; //FIXME (Give this a real type)

			GLint context;

			double pixelRatio = 1;

			std::vector<Light*> lights;

			std::vector<Object3D*> opaqueObjects;
			//opaqueObjectsLastIndex?

			std::vector<Object3D*> transparentObjects;
			//transparentObjectsLastIndex?

			std::vector<Object3D*> opaqueImmediateObjects;

			std::vector<Object3D*> transparentImmediateObjects;

			float morphInfluences[8];

			std::vector<Sprite> sprites;
			std::vector<lensFlare> lensFlares;

			GLint currentProgram = NULL,
			      currentBuffer = NULL,
			      currentMaterialId = -1,
			      currentGeometryProgram = NULL,
			      currentCamera = NULL,

			      usedTextureUnits = 0,

			      viewportX = 0,
			      viewportY = 0,
			      viewportWidth = 0,//TODO
			      viewportHeight = 0, //TODO
			      currentWidth = 0,
			      currentHeight = 0;

			Frustrum frustrum;

			Matrix4 projScreenMatrix;

			//Vector3 vector3?

			Vector3 direction;

			bool lightsNeedUpdate = true;

			//Do we need this?
			struct {
				float ambient[3];
				struct {
					float length 0;
					std::vector<Color> colors;
					std::vector<Vector3> positions;
				} directional;
				//TODO
			} _lights;



		public:

			//Context

			//Clearing
			bool autoClear = true,
			     autoClearColor = true,
			     autoClearDepth = true,
			     autoClearStencil = true;

			//scene graph
			bool sortObjects = true;

			//Physically based shading
			float gammaFactor = 2.0;
			bool gammaInput = false,
			     gammaOutput = false;

			//Morphs
			unsigned maxMorphTargets = 8,
				 maxMorphNormals = 4;

			//Flags
			bool autoScaleCubemaps = true;

			//Info
			struct {
				struct {
					unsigned calls = 0,
					 	 verticies = 0,
					 	 faces = 0,
					 	 points = 0;
				} renderer;
				struct {
				       unsigned geometries = 0,
						textures = 0;
				} memory;
			} info;


		};

	}

}

#endif

