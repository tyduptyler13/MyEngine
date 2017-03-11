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
		class Texture;

		namespace Shader {

			/**
			 * R - Renderer
			 * T - Type for conversion to some shading language.
			 * @return A renderer specific type related to a c type.
			 */
			template <class R, typename T>
			constexpr const char* type = "";

			//Specializations for shaders (Allows a renderer to work)
			template <> constexpr const char* type<GLES2Renderer, bool> = "bool";
			template <> constexpr const char* type<GLES2Renderer, int> = "int";
			template <> constexpr const char* type<GLES2Renderer, unsigned> = "uint";
			template <> constexpr const char* type<GLES2Renderer, float> = "float";
			template <> constexpr const char* type<GLES2Renderer, Vector2<float>> = "vec2";
			template <> constexpr const char* type<GLES2Renderer, Vector3<float>> = "vec3";
			template <> constexpr const char* type<GLES2Renderer, Vector4<float>> = "vec4";
			template <> constexpr const char* type<GLES2Renderer, Vector2<int>> = "ivec2";
			template <> constexpr const char* type<GLES2Renderer, Vector3<int>> = "ivec3";
			template <> constexpr const char* type<GLES2Renderer, Vector4<int>> = "ivec3";
			template <> constexpr const char* type<GLES2Renderer, Vector2<unsigned>> = "uvec2";
			template <> constexpr const char* type<GLES2Renderer, Vector3<unsigned>> = "uvec3";
			template <> constexpr const char* type<GLES2Renderer, Vector4<unsigned>> = "uvec4";
			template <> constexpr const char* type<GLES2Renderer, Matrix3<float>> = "mat3";
			template <> constexpr const char* type<GLES2Renderer, Matrix4<float>> = "mat4";
			template <> constexpr const char* type<GLES2Renderer, Texture> = "sampler2D";

			/**
			 * R - Renderer
			 * T - Type for conversion to some shading language.
			 * @return A renderer specific type related to a c type.
			 */
			template <class R, typename T>
			std::string toString(const T& t){
				return std::to_string(t); //Default fallback
			}

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
				virtual void prepare(unsigned contextID, Camera<float>*, Mesh<float>*, const std::vector<Light<float>*>& lights) = 0;

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

				virtual void traverseChildren(ShaderTraverser s) const override {
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

				void traverseChildren(ShaderTraverser s) const override {
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

				void traverseChildren(ShaderTraverser s) const override {
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

				void traverseChildren(ShaderTraverser s) const override {
					s(finalColor.node);
				}
			};


			//Things below this line are renderer specific, they may be moved at any time.

			//Function specializations must be defined in the header for the compiler to know they exist. They will go unused in a cpp file.
			template <>
			inline std::string toString<GLES2Renderer, bool>(const bool& t){
				if (t){
					return "true";
				} else {
					return "false";
				}
			}

			template <>
			inline std::string toString<GLES2Renderer, double>(const double& t){
				return std::to_string(t);
			}

			template <>
			inline std::string toString<GLES2Renderer, float>(const float& t){
				return std::to_string(t);
			}

			template <>
			inline std::string toString<GLES2Renderer, Vector3<float>>(const Vector3<float>& t){
				return "vec3(" + toString<GLES2Renderer, float>(t.x) + "," + toString<GLES2Renderer, float>(t.y) + "," + toString<GLES2Renderer, float>(t.z) + ")";
			}

			template <>
			inline std::string toString<GLES2Renderer, Vector4<float>>(const Vector4<float>& t){
				return "vec4(" + toString<GLES2Renderer, float>(t.x) + "," + toString<GLES2Renderer, float>(t.y) + "," + toString<GLES2Renderer, float>(t.z) + "," + toString<GLES2Renderer, float>(t.w) + ")";
			}

			template <>
			inline std::string toString<GLES2Renderer, Matrix3<float>>(const Matrix3<float>& t){
				std::string s = "mat3(" + toString<GLES2Renderer, float>(t.elements[0]);
				for (unsigned i = 1; i < 9; ++i){
					s += "," + toString<GLES2Renderer, float>(t.elements[i]);
				}
				s += ")";
				return s;
			}

			template <>
			inline std::string toString<GLES2Renderer, Matrix4<float>>(const Matrix4<float>& t){
				std::string s = "mat4(" + toString<GLES2Renderer, float>(t.elements[0]);
				for (unsigned i = 1; i < 16; ++i){
					s += "," + toString<GLES2Renderer, float>(t.elements[i]);
				}
				s += ")";
				return s;
			}

			template <>
			inline std::string toString<GLES2Renderer, Vector3<double>>(const Vector3<double>& t){
				return "dvec3(" + toString<GLES2Renderer, double>(t.x) + "," + toString<GLES2Renderer, double>(t.y) + "," + toString<GLES2Renderer, double>(t.z) + ")";
			}

			template <>
			inline std::string toString<GLES2Renderer, Vector4<double>>(const Vector4<double>& t){
				return "dvec4(" + toString<GLES2Renderer, double>(t.x) + "," + toString<GLES2Renderer, double>(t.y) + "," + toString<GLES2Renderer, double>(t.z) + "," + toString<GLES2Renderer, double>(t.w) + ")";
			}

			//TODO dmats might not be supported, haven't checked specifics for GLES2
			template <>
			inline std::string toString<GLES2Renderer, Matrix3<double>>(const Matrix3<double>& t){
				std::string s = "dmat3(" + toString<GLES2Renderer, double>(t.elements[0]);
				for (unsigned i = 1; i < 9; ++i){
					s += "," + toString<GLES2Renderer, double>(t.elements[i]);
				}
				s += ")";
				return s;
			}

			template <>
			inline std::string toString<GLES2Renderer, Matrix4<double>>(const Matrix4<double>& t){
				std::string s = "dmat4(" + toString<GLES2Renderer, double>(t.elements[0]);
				for (unsigned i = 1; i < 16; ++i){
					s += "," + toString<GLES2Renderer, double>(t.elements[i]);
				}
				s += ")";
				return s;
			}

		}
	}
}


#endif
