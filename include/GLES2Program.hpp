#ifndef MYUPLAY_MYENGINE_GLES2Program
#define MYUPLAY_MYENGINE_GLES2Program

#include <vector>
#include <utility>
#include <string>
#include <memory>

#include "GLES2Renderer.hpp"
#include "Texture.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "Constants.hpp"

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
									doubleSided,
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

						  GLES2Program(const GLES2Renderer& renderer, const std::string& code, const std::shared_ptr<Material<float> > material, const RenderParam& parameters){

								std::string shadowMapTypeDefine = "SHADOWMAP_TYPE_BASIC";

								if (parameters.shadowMapType == PCFShadowMap){
									   shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF";
								} else if (parameters.shadowMapType == PCFSoftShadowMap){
									   shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF_SOFT";
								}

								std::string envMapTypeDefine = "ENVMAP_TYPE_CUBE";
								std::string envMapModeDefine = "ENVMAP_MODE_REFLECTION";
								std::string envMapBlendingDefine = "ENVMAP_BLENDING_MULTIPLY";

								if (parameters.envMap) {
									   switch(material->envMap.mapping){
											 case CubeReflectionMapping:
											 case CubeRefractionMapping:
												    envMapTypeDefine = "ENVMAP_TYPE_CUBE";
												    break;

											 case EquirectangularReflectionMapping:
											 case EquirectangularRefractionMapping:
												    envMapTypeDefine = "ENVMAP_TYPE_EQUIREC";
												    break;

											 case SphericalReflectionMapping:
												    envMapTypeDefine = "ENVMAP_TYPE_SPHERE";
												    break;
									   }

									   switch(material->envMap.mapping) {
											 case CubeRefractionMapping:
											 case EquirectangularRefractionMapping:
												    envMapModeDefine = "ENVMAP_MODE_REFRACTION";
												    break;
									   }

									   switch(material->combine){
											 case MultiplyOperation:
												    envMapBlendingDefine = "ENVMAP_BLENDING_MULTIPLY";
												    break;

											 case MixOperation:
												    envMapBlendingDefine = "ENVMAP_BLENDING_MIX";
												    break;

											 case AddOperation:
												    envMapBlendingDefine = "ENVMAP_BLENDING_ADD";
												    break;
									   }

								}

								float gammaFactorDefine = renderer.gammaFactor > 0 ? renderer.gammaFactor : 1.0;

								std::string customDefines = generateDefines(material->defines);

								GLuint program = glCreateProgram();

								std::string prefixVertex, prefixFragment;

								if (material->type == Material<float>::RAWSHADERMATERIAL){

									   prefixVertex = "";
									   prefixFragment = "";

								} else {

									   prefixVertex = 

											 "precision " + parameters.precision + " float;\n"
											 "precision " + parameters.precision + " int;\n"
											 "#define SHADER_NAME " + material->shaderData["GLES2"]["name"] + "\n"
											 customDefines + "\n"

											 parameters.supportsVertexTextures ? "#define VERTEX_TEXTURES\n" : ""
											 renderer.gammaInput ? "#define GAMMA_INTPUT\n": ""
											 renderer.gammaOutput ? "#define GAMMA_OUTPUT\n": ""
											 "#define GAMMA_FACTOR " + std::to_string(gammaFactorDefine) + "\n"
											 "#define MAX_DIR_LIGHTS " + parameters.maxDirLights + "\n"
											 "#define MAX_POINT_LIGHTS " + parameters.maxPointLights + "\n"
											 "#define MAX_SPOT_LIGHTS " + parameters.maxSpotLights + "\n"
											 "#define MAX_HEMI_LIGHTS " + parameters.maxHemiLights + "\n"
											 "#define MAX_SHADOWS " + parameters.maxShadows + "\n"
											 "#define MAX_BONES " + parameters.maxBones + "\n"
											 parameters.map ? "#define USE_MAP\n" : ""
											 parameters.envMap ? "#define USE_ENVMAP\n" : ""
											 parameters.envMap ? "#define " + envMapModeDefine + "\n" : ""
											 parameters.lightMap ? "#define USE_LIGHTMAP\n" : ""
											 parameters.aoMap ? "#define USE_AOMAP\n" : ""
											 parameters.emissiveMap ? "#define USE_EMISSIVEMAP\n" : ""
											 parameters.bumpMap ? "#define USE_BUMBMAP\n" : ""
											 parameters.normalMap ? "#define USE_NORMALMAP\n" : ""
											 parameters.displacementMap && parameters.supportsVertexTextures ? "#define USE_DISPLACEMENTMAP\n" : ""
											 parameters.specularMap ? "#define USE_SPECULARMAP\n" : ""
											 parameters.alphaMap ? "#define USE_ALPHAMAP\n" : ""
											 parameters.vertexColors ? "#define USE_COLOR\n" : ""
											 parameters.flatShading ? "#define FLAT_SHADED\n" : "" 
											 parameters.skinning ? "#define USE_SKINNING\n" : ""
											 parameters.useVertextTexture ? "#define BONE_TEXTURE\n" : ""
											 parameters.morphTargets ? "#define USE_MORPHTARGETS\n" : ""
											 parameters.morphNormals && parameters.flatShading == false ? "#define USE_MORPHNORMALS\n" : ""
											 parameters.doubleSided ? "#define DOUBLE_SIDED\n" : ""
											 parameters.flipSided ? "#define FLIP_SIDED\n" : ""
											 parameters.shadowMapEnabled ? "#define USE_SHADOWMAP\n" : ""
											 parameters.shadowMapEnabled ? "#define " + shadowMapTypeDefine + "\n" : ""
											 parameters.shadowMapDebug ? "#define SHADOWMAP_DEBUG\n" : ""
											 parameters.pointLightShadows ? "#define POINT_LIGHT_SHADOWS\n" : ""
											 parameters.sizeAttenuation ? "#define USE_SIZEATTENTUATION\n" : ""
											 parameters.logarithmicDepthBuffer ? "#define USE_LOGDEPTHBUF\n" : ""
											 parameters.logarithmicDepthBuffer && renderer.extentions.get(GL_EXT_FRAG_DEPTH) ? "#define USE_LOGDEPTHBUF_EXT\n" : ""

											 "uniform mat4 modelMatrix;\n"
											 "uniform mat4 modelViewMatrix;\n"
											 "uniform mat4 projectionMatrix;\n"
											 "uniform mat4 viewMatrix;\n"
											 "uniform mat3 normalMatrix;\n"
											 "uniform vec3 cameraPosition;\n"

											 "attribute vec3 position;\n"
											 "attribute vec3 normal;\n"
											 "attribute vec2 uv;\n"

											 "#ifdef USE_COLOR\n"

											 "       attribute vec3 color;\n"

											 "#endif\n"

											 "#ifdef USE_MORPHTARGETS\n"

											 "       attribute vec3 morphTarget0;\n"
											 "       attribute vec3 morphTarget1;\n"
											 "       attribute vec3 morphTarget2;\n"
											 "       attribute vec3 morphTarget3;\n"

											 "       #ifdef USE_MORPHNORMALS\n"

											 "               attribute vec3 morphNormal0;\n"
											 "               attribute vec3 morphNormal1;\n"
											 "               attribute vec3 morphNormal2;\n"
											 "               attribute vec3 morphNormal3;\n"

											 "       #else\n"

											 "               attribute vec3 morphTarget4;\n"
											 "               attribute vec3 morphTarget5;\n"
											 "               attribute vec3 morphTarget6;\n"
											 "               attribute vec3 morphTarget7;\n"

											 "       #endif\n"

											 "#endif\n"

											 "#ifdef USE_SKINNING\n"

											 "       attribute vec4 skinIndex;\n"
											 "       attribute vec4 skinWeight;\n"

											 "#endif\n"

											 "\n";

									   prefixFragment =

											 "";

								}
						  }

			 };

	   }

}

#endif

