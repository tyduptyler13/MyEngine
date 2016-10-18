#ifndef SHADER_SHADERLIB
#define SHADER_SHADERLIB

#include <string>

#include "Shader/ShaderNode.hpp"

#include "Vector3.hpp"
#include "Matrix4.hpp"

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			//Specialized for GLES2
			struct TransformDirection : public IShaderNode<GLES2Renderer> {

				Input<GLES2Renderer, Vector3<float>> dir;
				Input<GLES2Renderer, Matrix4<float>> matrix;

				Output<GLES2Renderer, Vector3<float>> ret;

				std::string getStatic() const override {
					return "vec3 transformDirection(in vec3 dir, in mat4 matrix) {\n"
							"	normalize((matrix * vec4(dir, 0.0)).xyz);\n"
							"}\n";
				}
				std::string getInstance() const override {
					return ret.name + " = transformDirection(" + dir.output->name + ", " + matrix.output->name + ");\n";
				}

			};

			template <typename T>
			struct Add : public IShaderNode<GLES2Renderer> {

				Input<GLES2Renderer, T> a;
				Input<GLES2Renderer, T> b;

				Output<GLES2Renderer, T> ret;

				std::string getInstance() const override {
					return ret.name + " = " + a.output->name + " + " + b.output->name + ";\n";
				}

			};

			template <typename T>
			struct Mix : public IShaderNode<GLES2Renderer> {
				Input<GLES2Renderer, T> a;
				Input<GLES2Renderer, T> b;
				Input<GLES2Renderer, float> fac; //Factor from 0 to 1 for all a to all b;
				//Undefined behavior for values outside of 0 and 1;

				Output<GLES2Renderer, T> ret;

				std::string getInstance() const override {
					//mix should already be defined.
					return ret.name + " = mix(" + a.output->name + ", " + b.output->name + ", "
							+ fac.output->name + ");\n";
				}

			};

			/**
			 * A simple create a new variable based on an existing value.
			 */
			template <typename T>
			struct Variable : public IShaderNode<GLES2Renderer> {
				Input<GLES2Renderer, T> in;
				Output<GLES2Renderer, T> out;

				std::string getStatic() const override {
					return Utility<GLES2Renderer, T>::type + " " + out.name  + " = " + in.output->name + ";\n";
				} //TODO replace to_string with internal function that handles vector3-4 and matrix3-4, etc.

				std::string getInstance() const override {
					return out.name;
				}
			};

			/**
			 * Create a new static variable based on initial input. (Must be recompiled every time the value changes.
			 *
			 * If you are looking to change this value every frame, then use an attribute.
			 */
			template <typename T>
			struct InputVariable : public IShaderNode<GLES2Renderer> {

				T value; //Note, this will only be used at the time the shader is compiled. You must recompile every time this changes.

				Output<GLES2Renderer, T> out;

				std::string getStatic() const override {
					return "const " + Utility<GLES2Renderer, T>::type + " " + out.name  + " = " + std::to_string(value) + ";\n";
				} //TODO replace to_string with internal function that handles vector3-4 and matrix3-4, etc.

				std::string getInstance() const override {
					return out.name;
				}

			};

			struct Fresnel : public IShaderNode<GLES2Renderer> {

				Input<GLES2Renderer, float> fac;
				Input<GLES2Renderer, Vector3<float>> normal;
				Input<GLES2Renderer, Vector3<float>> lightDir;

				Output<GLES2Renderer, float> out;

				std::string getStatic() const override {
					return "float fresnelSchlick(vec3 norm, vec3 lightDir, float fac){\n"
							"	float base = 1.0 - dot(norm, lightDir);\n"
							"	float exponential = pow(base, 5.0);\n"
							"	return exponential + fac * (1.0 - exponential);"
							"}\n";
				}

				std::string getInstance() const override {
					return out.name + " = fresnelSchlick(" + normal.output->name + ", "
							+ lightDir.output->name + ", " + fac.output->name + ");\n";
				}

			};

			struct Rand : public IShaderNode<GLES2Renderer> {

				Input<GLES2Renderer, Vector2<float>> seed;

				Output<GLES2Renderer, float> out;

				std::string getStatic() const override {
					return "float rand(vec2 co){\n"
							"	float a = 12.9898;\n"
							"	float b = 78.233;\n"
							"	float c = 43758.5453;\n"
							"	float dt= dot(co.xy ,vec2(a,b));\n"
							"	float sn= mod(dt,3.14);\n"
							"	return fract(sin(sn) * c);\n"
							"}\n";
				}

				std::string getInstance() const override {
					return out.name + " = rand(" + seed.output->name + ");\n";
				}

			};



			struct DielectricShader : public IShaderNode<GLES2Renderer> {

			};

			struct MetallicShader : public IShaderNode<GLES2Renderer> {

			};



		}
	}
}

#endif
