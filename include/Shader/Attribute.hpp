/*
 * ShaderAttribute.hpp
 *
 *  Created on: Jul 13, 2016
 *      Author: tyler
 */

#ifndef MyUPlay_MyEngine_Attribute
#define MyUPlay_MyEngine_Attribute

#include <array>
#include <functional>

#include "Shader/ShaderUtil.hpp"

namespace MyUPlay{
	namespace MyEngine {

		class GLES2Renderer;

		namespace Shader {

			template <class R>
			struct IShaderNode;

			template <class R, typename T>
			struct Output;

			enum ShaderScope {
				PerPrimitive,
				PerVertex
			};

			template <class R>
			struct IAttribute : public IShaderNode<R> {
				ShaderScope scope;
				bool dirty = true;

				virtual ~IAttribute(){}

				//This is expected to set the dirty flag to false.
				virtual void push() = 0; //Completely renderer specific.

				//This will be called before every shader process.
				virtual void bind() = 0;

				/**
				 * Attributes don't have instances, instead you use its name.
				 */
				virtual std::string getInstance() const override {
					return "";
				}

			};

			/**
			 * Attribute is a class meant to take c variables and push them to the gpu.
			 * This class is heavily templated to allow any shader language to use it but
			 * then it must have the static variables/methods created for that renderer.
			 *
			 * Template:
			 * Renderer followed by the type the class holds.
			 * This will be automatically converted thanks to specializations in each render class
			 * Size: We put size in the template to allow some quick optimization and prevent users
			 * from changing attribute sizes over time. If you want to change "size" then create a
			 * max size one and a second that indicates its length. Otherwise the shader needs to
			 * be recompiled every time the size changes.
			 *
			 * @param name - Must be unique to the shader.
			 * @param value - The initial value to use.
			 */
			template <class R, typename T, unsigned size>
			class MappedArrayAttribute : public IAttribute<R> {
			private:
				std::array<T, size> value;
			public:

				Output<R, T> out;

				MappedArrayAttribute(const std::string& name, ShaderScope s, const std::array<T, size>& value)
				: value(value), IShaderNode<R>::scope(s) {
					out.name = name;
				}
				MappedArrayAttribute(std::string&& name, ShaderScope s, std::array<T,size>&& value)
				: value(value), IShaderNode<R>::scope(s) {
					out.name = name;
				}
				~MappedArrayAttribute();

				//This must be defined with a partial specialization in each renderer R
				std::string getStatic() const override {
					std::string ret;
					switch(this->scope){
						case PerPrimitive:
						ret = "uniform";
						break;
						case PerVertex:
						ret = "attribute";
						break;
					}
					ret += " " + Utility<R, T>::type + " " + out.name + "[" + size + "];";
					return ret;
				}

				void setValue(const std::array<T, size>& value){
					this->dirty = true;
					this->value = value;
				}
				void setValue(std::array<T, size>&& value){
					this->dirty = true;
					this->value = value;
				}

				void push() override;
				void bind() override;

			};

			template <class R, typename T>
			class Attribute : public IAttribute<R> {
			private:
				T value;
			public:

				Output<R, T> out;

				Attribute(const std::string& name, ShaderScope s, const T& value) : value(value), IShaderNode<R>::s(s) {
					out.name = name;
				}
				Attribute(std::string&& name, ShaderScope s, T&& value) : value(value), IShaderNode<R>::s(s) {
					out.name = name;
				}
				~Attribute();

				//This must be defined with a partial specialization in each renderer R
				std::string getStatic() const override {
					std::string ret;
					switch(this->scope){
						case PerPrimitive:
						ret = "uniform";
						break;
						case PerVertex:
						ret = "attribute";
						break;
					}
					ret += " " + Utility<R, T>::type + " " + out.name + ";";
					return ret;
				}

				void setValue(const T& val){
					value = val;
				}

				void setValue(T&& val){
					value = val;
				}

				void push() override;
				void bind() override;

			};

		}
	}
}

//Circular
#include "Shader/ShaderNode.hpp"


#endif /* INCLUDE_SHADER_SHADERATTRIBUTE_HPP_ */
