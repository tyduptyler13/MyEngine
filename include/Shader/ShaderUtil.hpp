/*
 * ShaderUtil.hpp
 *
 *  Created on: Jul 20, 2016
 *      Author: tyler
 */

#ifndef MYUPLAY_MYENGINE_SHADERUTIL
#define MYUPLAY_MYENGINE_SHADERUTIL

#include "Shader/ShaderNode.hpp"
#include "Shader/Attribute.hpp"

#include "Mesh.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Light.hpp"

#include <string>

namespace MyUPlay {
	namespace MyEngine {

		struct GLES2Renderer;

		namespace Shader {

			/**
			 * R - Renderer
			 * T - Type for conversion to some shading language.
			 * @return A renderer specific type related to a c type.
			 */
			template <typename R, typename T>
			struct Utility {

				//Types can be hidden away in cpp files. Currently these are defined in each renderers cpp file.
				static const char* type;
				static const char* getType(){
					return type;
				}

				inline static std::string toString(const T& t){
					return std::to_string(t); //Default fallback
				}

			};

			/**
			 * Root shaders are special shader nodes that can provide both inputs and outputs.
			 * These nodes are where the compiler will start and usually end.
			 */
			struct IRootShaderNode : public IShaderNode {

				virtual ~IRootShaderNode(){}

				std::vector<std::unique_ptr<IAttribute>> customAttributes;

				/**
				 * This function must be overriden by each root node and by every renderer to supply
				 * the shader with the required attributes that will ALWAYS be part of the code.
				 * Should the attribute get optimized out because it wasn't used, only then can
				 * it be skipped. In opengl this is done when the position is not present in the shader.
				 */
				virtual void prepare(Camera<float>*, Mesh<float>*, const std::vector<Light<float>*>& lights) = 0;

			};

			/**
			 * This node is where the core vertex shader components attach.
			 */
			struct VertexBase : public IRootShaderNode {

				Output<Matrix4f> modelView;
				Output<Matrix4f> projectionMatrix;
				Output<Matrix4f> modelMatrix; //Matrix world
				Output<Matrix4f> viewMatrix; //Inverse matrix world
				Output<Vector3f> cameraPosition;

				// The following are treated as varying in GLSL and passed to the fragment shader
				Input<Vector3f> fPosition; //World coords
				Input<Vector3f> fNormal; //Output the normalized normal vector for use in the fragment shader.

				virtual void traverseChildren(ShaderTraverser s) override {
					s(fPosition.node);
					s(fNormal.node);
				}

			protected:

				//Default constructor should not be directly callable.
				VertexBase() : modelView("modelView"), projectionMatrix("projectionMatrix"), modelMatrix("modelMatrix"),
				viewMatrix("viewMatrix"), cameraPosition("cameraPosition") {}

			};

			struct FragmentBase : public IRootShaderNode {
				Output<Vector3f> position; //Interpolated world position from vertex shader
				Output<Vector3f> normal; //Interpolated normal from vertex shader (normalized)

				//The following are combined in opengl automatically.
				Input<Vector3f> color;
				Input<float> alpha;

				void traverseChildren(ShaderTraverser s) override {
					s(color.node);
				}

			protected:

				FragmentBase() : position("fPosition"), normal("fNormal") {}

			};

			struct DeferredFragment : public IRootShaderNode {
				Output<Vector3f> vposition; // Position from vertex shader
				Output<Vector3f> vnormal; //Normal from vertex shader.

				Input<Vector3f> position; //Camera space position
				Input<Vector3f> normal; //Camera space normal
				Input<Color> color;
				Input<Color> emission;

				void traverseChildren(ShaderTraverser s) override {
					s(position.node);
					s(normal.node);
					s(color.node);
					s(emission.node);
				}
			};

			struct DeferredLightingPass : public IRootShaderNode {
				Output<Vector3f> position;
				Output<Vector3f> normal;
				Output<Color> objColor;
				Output<Color> objEmission;

				Input<Color> finalColor;

				void traverseChildren(ShaderTraverser s) override {
					s(finalColor.node);
				}
			};


			//Things below this line are renderer specific, they may be moved at any time.

			//Function specializations must be defined in the header for the compiler to know they exist. They will go unused in a cpp file.
			template <>
			inline std::string Utility<GLES2Renderer, bool>::toString(const bool& t){
				if (t){
					return "true";
				} else {
					return "false";
				}
			}

			template <>
			inline std::string Utility<GLES2Renderer, double>::toString(const double& t){
				return std::to_string(t);
			}

			template <>
			inline std::string Utility<GLES2Renderer, float>::toString(const float& t){
				return std::to_string(t);
			}

			template <>
			inline std::string Utility<GLES2Renderer, Vector3<float>>::toString(const Vector3<float>& t){
				return "vec3(" + Utility<GLES2Renderer, float>::toString(t.x) + "," + Utility<GLES2Renderer, float>::toString(t.y) + "," + Utility<GLES2Renderer, float>::toString(t.z) + ")";
			}

			template <>
			inline std::string Utility<GLES2Renderer, Vector4<float>>::toString(const Vector4<float>& t){
				return "vec4(" + Utility<GLES2Renderer, float>::toString(t.x) + "," + Utility<GLES2Renderer, float>::toString(t.y) + "," + Utility<GLES2Renderer, float>::toString(t.z) + "," + Utility<GLES2Renderer, float>::toString(t.w) + ")";
			}

			template <>
			inline std::string Utility<GLES2Renderer, Matrix3<float>>::toString(const Matrix3<float>& t){
				std::string s = "mat3(" + Utility<GLES2Renderer, float>::toString(t.elements[0]);
				for (unsigned i = 1; i < 9; ++i){
					s += "," + Utility<GLES2Renderer, float>::toString(t.elements[i]);
				}
				s += ")";
				return s;
			}

			template <>
			inline std::string Utility<GLES2Renderer, Matrix4<float>>::toString(const Matrix4<float>& t){
				std::string s = "mat4(" + Utility<GLES2Renderer, float>::toString(t.elements[0]);
				for (unsigned i = 1; i < 16; ++i){
					s += "," + Utility<GLES2Renderer, float>::toString(t.elements[i]);
				}
				s += ")";
				return s;
			}

			template <>
			inline std::string Utility<GLES2Renderer, Vector3<double>>::toString(const Vector3<double>& t){
				return "dvec3(" + Utility<GLES2Renderer, double>::toString(t.x) + "," + Utility<GLES2Renderer, double>::toString(t.y) + "," + Utility<GLES2Renderer, double>::toString(t.z) + ")";
			}

			template <>
			inline std::string Utility<GLES2Renderer, Vector4<double>>::toString(const Vector4<double>& t){
				return "dvec4(" + Utility<GLES2Renderer, double>::toString(t.x) + "," + Utility<GLES2Renderer, double>::toString(t.y) + "," + Utility<GLES2Renderer, double>::toString(t.z) + "," + Utility<GLES2Renderer, double>::toString(t.w) + ")";
			}

			//TODO dmats might not be supported, haven't checked specifics for GLES2
			template <>
			inline std::string Utility<GLES2Renderer, Matrix3<double>>::toString(const Matrix3<double>& t){
				std::string s = "dmat3(" + Utility<GLES2Renderer, double>::toString(t.elements[0]);
				for (unsigned i = 1; i < 9; ++i){
					s += "," + Utility<GLES2Renderer, double>::toString(t.elements[i]);
				}
				s += ")";
				return s;
			}

			template <>
			inline std::string Utility<GLES2Renderer, Matrix4<double>>::toString(const Matrix4<double>& t){
				std::string s = "dmat4(" + Utility<GLES2Renderer, double>::toString(t.elements[0]);
				for (unsigned i = 1; i < 16; ++i){
					s += "," + Utility<GLES2Renderer, double>::toString(t.elements[i]);
				}
				s += ")";
				return s;
			}

		}
	}
}


#endif
