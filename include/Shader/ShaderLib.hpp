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
			class TransformDirection : public IShaderNode<GLES2Renderer> {
			public:

				Input<GLES2Renderer, Vector3<float>> dir;
				Input<GLES2Renderer, Matrix4<float>> matrix;

				Output<GLES2Renderer, Vector3<float>> ret;

				TransformDirection() : IShaderNode(ShaderScope::Any) {} //Default constructor

				std::string getStatic() const override {
					return "vec3 transformDirection(in vec3 dir, in mat4 matrix) {\n"
					"	normalize((matrix * vec4(dir, 0.0)).xyz);\n"
					"}\n";
				}
				std::string getInstance() const override {
					return ret.name + " = transformDirection(" + dir.output->name + ", " + matrix.output->name + ");\n";
				}

				virtual void makeDirty() {
					dirty = true;
					IShaderNode<GLES2Renderer>::makeDirty(ret.inputs);
				}

			};

			template <typename T>
			class Add : public IShaderNode<GLES2Renderer> {
			public:

				Input<GLES2Renderer, T> a;
				Input<GLES2Renderer, T> b;

				Output<GLES2Renderer, T> ret;

				Add() : IShaderNode(ShaderScope::Any) {}

				std::string getInstance() const override {
					return ret.name + " = " + a.output->name + " + " + b.output->name + ";\n";
				}

				virtual void makeDirty() {
					dirty = true;
					IShaderNode<GLES2Renderer>::makeDirty(ret.inputs);
				}

			};

			template <typename T>
			class Mix : public IShaderNode<GLES2Renderer> {
			public:
				Input<GLES2Renderer, T> a;
				Input<GLES2Renderer, T> b;
				Input<GLES2Renderer, float> fac; //Factor from 0 to 1 for all a to all b;
				//Undefined behavior for values outside of 0 and 1;

				Output<GLES2Renderer, T> ret;

				Mix() : IShaderNode(ShaderScope::Any) {}

				std::string getInstance() const override {
					//ret = a * (1 - fac) + b * fac;
					return ret.name + " = " + a.output->name + " * (1 - " + fac.output->name + ") + "
					 + b.output->name + " * " + fac.output->name + ";\n";
				}

				virtual void makeDirty() {
					dirty = true;
					IShaderNode<GLES2Renderer>::makeDirty(ret.inputs);
				}
			};

			template <typename T>
			class InputVariable : public IShaderNode<GLES2Renderer> {

				T value; //Note, this will only be used at the time the shader is compiled. You must recompile every time this changes.

				Output<GLES2Renderer, T> out;

				InputVariable() : IShaderNode(ShaderScope::Any) {}

				std::string getStatic() const override {
					return "const " + Utility<GLES2Renderer, T>::type + " " + out.name  + " = " + std::to_string(value) + ";\n";
				} //TODO replace to_string with internal function that handles vector3-4 and matrix3-4, etc.

				std::string getInstance() const override {
					return out.name;
				}

				virtual void makeDirty() {
					dirty = true;
					IShaderNode<GLES2Renderer>::makeDirty(out.inputs);
				}

			};

			class BlinnPhong : public IShaderNode<GLES2Renderer> {

				Input<Vector3<float>> position;
				Input<Vector3<float>> normal;

				//TODO

			};

		}
	}
}

#endif
