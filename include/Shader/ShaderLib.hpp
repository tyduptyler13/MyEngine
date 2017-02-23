#ifndef SHADER_SHADERLIB
#define SHADER_SHADERLIB

#include <string>

#include "Shader/ShaderNode.hpp"
#include "Shader/ShaderUtil.hpp"

#include "Vector3.hpp"
#include "Matrix4.hpp"

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			/**
			 * The following are expected to change soon to allow for multiple renderers.
			 *
			 * Expect some form of factory system that produces the desired node, specialized for a renderer of choice.
			 *
			 * The current design is mainly for proof of concept testing.
			 */

			//Specialized for GLES2
			struct TransformDirection : public IShaderNode {

				Input<Vector3<float>> dir;
				Input<Matrix4<float>> matrix;

				Output<Vector3<float>> ret;

				std::string getStatic() const override {
					return "vec3 transformDirection(in vec3 dir, in mat4 matrix) {\n"
							"	return normalize((matrix * vec4(dir, 0.0)).xyz);\n"
							"}\n";
				}
				std::string getInstance() const override {
					return "vec3 " + ret.name + " = transformDirection(" + dir.output->name + ", " + matrix.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(dir.node);
					s(matrix.node);
				}

			};

			//Specialized for GLES2
			struct TransformLocation : public IShaderNode {

				Input<Vector3<float>> loc;
				Input<Matrix4<float>> matrix;

				Output<Vector3<float>> ret;

				std::string getStatic() const override {
					return "vec3 transformLocation(in vec3 loc, in mat4 matrix) {\n"
							"	return (matrix * vec4(loc, 1.0)).xyz;\n"
							"}\n";
				}
				std::string getInstance() const override {
					return "vec3 " + ret.name + " = transformLocation(" + loc.output->name + ", " + matrix.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(loc.node);
					s(matrix.node);
				}

			};

			template <typename T>
			struct Add : public IShaderNode {

				Input<T> a;
				Input<T> b;

				Output<T> ret;

				std::string getInstance() const override {
					return type<GLES2Renderer, T> + " " + ret.name + " = " + a.output->name + " + " + b.output->name + ";\n";
				}

				void traverseChildren(ShaderTraverser s) const override{
					s(a.node);
					s(b.node);
				}

			};

			//It is up to the designer to know if the types inserted are valid.
			template <typename A, typename B, typename R>
			struct Multiply : public IShaderNode {

				Input<A> a;
				Input<B> b;

				Output<R> ret;

				std::string getInstance() const override {
					return std::string(type<GLES2Renderer, R>) + " " + ret.name + " = " + a.output->name + " * " + b.output->name + ";\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(a.node);
					s(b.node);
				}

			};

			//It is up to the designer to know if the types inserted are valid.
			template <typename A, typename B, typename R>
			struct Combine : public IShaderNode {

				Input<A> a;
				Input<B> b;

				Output<R> ret;

				std::string getInstance() const override {
					return std::string(type<GLES2Renderer, R>) + " " + ret.name + " = " +
							type<GLES2Renderer, R> + "(" + a.output->name + ", " + b.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(a.node);
					s(b.node);
				}

			};

			//It is up to the designer to know if the types inserted are valid.
			/**
			 * This class is designed to get sub elements of a variable to create a new one.
			 */
			template <class A, typename R>
			struct Swizzle : public IShaderNode {

				Input<A> a;
				std::string swiz = ".x"; //By default returns only x.

				Output<R> ret;

				std::string getInstance() const override {
					return type<GLES2Renderer, R> + " " + ret.name + " = " + a.output->name + swiz + ";\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(a.node);
				}

			};

			template <typename T>
			struct Mix : public IShaderNode {
				Input<T> a;
				Input<T> b;
				Input<float> fac; //Factor from 0 to 1 for all a to all b;
				//Undefined behavior for values outside of 0 and 1;

				Output<T> ret;

				std::string getInstance() const override {
					//mix should already be defined.
					return type<GLES2Renderer, T> + " " + ret.name + " = mix(" + a.output->name +
							", " + b.output->name + ", " + fac.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(a.node);
					s(b.node);
					s(fac.node);
				}

			};

			/**
			 * Create a new static variable based on initial input. (Must be recompiled every time the value changes.
			 *
			 * If you are looking to change this value every frame, then use an attribute.
			 */
			template <typename T>
			struct InputVariable : public IShaderNode {

				T value; //Note, this will only be used at the time the shader is compiled. You must recompile every time this changes.

				Output<T> out;

				std::string getStatic() const override {
					return std::string("const ") + type<GLES2Renderer, T> + " " + out.name  + " = " + toString<GLES2Renderer, T>(value) + ";\n";
				}

				std::string getInstance() const override {
					return ""; //Other nodes will use the out, not the get instance method.
				}

				InputVariable(T t) : value(t){}

			};

			/* Incorrect, shouldn't depend on light position.
			struct Fresnel : public IShaderNode {

				Input<float> fac;
				Input<Vector3<float>> normal;
				Input<Vector3<float>> lightDir;

				Output<float> out;

				std::string getStatic() const override {
					return "float fresnelSchlick(vec3 norm, vec3 lightDir, float fac){\n"
							"	float base = 1.0 - dot(norm, lightDir);\n"
							"	float exponential = pow(base, 5.0);\n"
							"	return exponential + fac * (1.0 - exponential);"
							"}\n";
				}

				std::string getInstance() const override {
					return "float " + out.name + " = fresnelSchlick(" + normal.output->name + ", "
							+ lightDir.output->name + ", " + fac.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const{
					s(fac);
					s(normal);
					s(lightDir);
				}

			};
			 */

			struct Rand : public IShaderNode {

				Input<Vector2<float>> seed;

				Output<float> out;

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
					return "float " + out.name + " = rand(" + seed.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(seed.node);
				}

			};

			struct MapFragment : public IShaderNode {

				Input<Vector2<float>> uv;
				Input<unsigned> map;

				Output<Vector4<float>> color;

				std::string getInstance() const override {
					return "vec4 " + color.name + " = texture2D(" + map.output->name + ", " + uv.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(uv.node);
					s(map.node);
				}

			};

			/**
			 * Per-Pixel Tangent Space Normal Mapping
			 * http://hacksoflife.blogspot.ch/2009/11/per-pixel-tangent-space-normal-mapping.html
			 */
			struct PerturbNormal2Arb : public IShaderNode {

				Input<Vector3<float>> eye_pos;
				Input<Vector3<float>> surf_norm;
				Input<unsigned> normalMap;
				Input<Vector2<float>> normalScale;
				Input<Vector2<float>> uv;

				Output<Vector3<float>> out;

				std::string getStatic() const override {
					return "#extension GL_OES_standard_derivatives : enable\n"
							"\n"
							"vec3 PerturbNormal2Arb(vec3 eye_pos, vec3 surf_norm, sampler2D normalMap, vec2 normalScale, vec2 uv){\n"
							"	vec3 q0 = dFdx( eye_pos.xyz );\n"
							"	vec3 q1 = dFdy( eye_pos.xyz );\n"
							"	vec2 st0 = dFdx ( uv.st );\n"
							"	vec2 st1 = dFdy ( uv.st );\n"
							"\n"
							"	vec3 S = normalize( q0 * st1.t - q1 * st0.t );\n"
							"	vec3 T = normalize( -q0 * st1.s + q1 * st0.s );\n"
							"	vec3 N = normalize( surf_norm );\n"
							"\n"
							"	vec3 mapN = texture2D( normalMap, uv ).xyz * 2.0 - 1.0;\n"
							"	mapN.xy = normalScale * mapN.xy;\n"
							"	mat3 tsn = mat3(S, T, N);\n"
							"	return normalize( tsn * mapN );\n"
							"}\n";
				}

				std::string getInstance() const override {
					return "vec3 " + out.name + " = PerturbNormal2Arb(" +
							eye_pos.output->name + ", " +
							surf_norm.output->name + ", " +
							normalMap.output->name + ", " +
							normalScale.output->name + ", " +
							uv.output->name + ");\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(eye_pos.node);
					s(surf_norm.node);
					s(normalMap.node);
					s(normalScale.node);
					s(uv.node);
				}

			};


			struct DielectricShader : public IShaderNode {
				//TODO
			};

			struct MetallicShader : public IShaderNode {
				//TODO
			};



		}
	}
}

#endif
