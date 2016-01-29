#ifndef MYUPLAY_MYENGINE_GLES2Program
#define MYUPLAY_MYENGINE_GLES2Program

#include <vector>
#include <utility>
#include <string>
#include <memory>

#include "GLES2Renderer.hpp"
#include "Texture.hpp"
#include "Color.hpp"

namespace MyUPlay {

	namespace MyEngine {

		//NOTE: GLES2Renderer must be loaded and constructed before this code can work.

		class GLES2Program : public ShaderProgram {

			private:

				static std::string generateDefines(const std::vector<std::pair<std::string, std::string> >& defines);
				static std::vector<std::pair<std::string, GLint> > fetchUniformLocations(GLuint program);
				static std::vector<std::pair<std::string, GLint> > fetchAttributeLocations(GLuint program);

			public:

				struct RenderParam {
					short shadowMapType;

					std::string precision;
					bool supportsVertexTextures;
					unsigned maxDirLights,
						 maxPointLights,
						 maxSpotLights,
						 maxHemiLights,
						 maxShadows,
						 maxBones;
					
					std::shared_ptr<Texture<float> > 
						map,
						envMap,
						lightMap,
						aoMap,
						emissiveMap,
						bumpMap,
						normalMap,
						displacementMap,
						specularMap,
						alphaMap;

					std::shared_ptr<std::vector<Color> > vertexColors;
					
					bool flatShading,
					     skinning,
					     useVertexTexture,
					     morphTargets, //Type?
					     morphNormals,
					     doubleSized,
					     flipSided,
					     shadowMapEnabled,
					     shadowMapDebug;

					std::shared_ptr<Texture<float> > pointLightShadows;

					float sizeAttenuation; //Type?

					bool logarithmicDepthBuffer;

				};

				GLES2Program(GLES2Renderer& renderer, const std::string& code, const Material<float>& material, const RenderParam& parameters);
				~GLES2Program();
				std::vector<std::pair<std::string, GLint> > getAttributes();
				std::vector<std::pair<std::string, GLint> > getUniforms();

				GLint program;
				//These get deleted in the constructor. Leftovers?
				//GLint vertexShader;
				//GLint fragmentShader;

		};

	}

}

#endif

