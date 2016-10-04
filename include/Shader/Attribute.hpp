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

			enum ShaderScope {
				PerPrimitive,
				PerVertex
			};

			template <class R>
			struct IAttribute : public IShaderNode<R> {
			protected:
				bool dirty = true;

			public:

				virtual ~IAttribute(){}

				bool isDirty() {
					return dirty;
				}

				//This is expected to set the dirty flag to false.
				virtual void push() = 0; //Completely renderer specific.

			};

			/**
			 * Attribute is a class meant to take c variables and push them to the gpu.
			 * This class is heavily templated to allow any shader language to use it but
			 * then it must have the static variables/methods created for that renderer.
			 * Note: Copying this is a shallow copy. You should create a new instance
			 * with its own static array/data for a deep copy.
			 *
			 * Template:
			 * Renderer followed by the type the class holds.
			 * This will be automatically converted thanks to specializations in each render class
			 * Size: We put size in the template to allow some quick optimization and prevent users
			 * from changing attribute sizes over time. If you want to change "size" then create a
			 * max size one and a second that indicates its length. Otherwise the shader needs to
			 * be recompiled every time the size changes. If you don't want to use an array then
			 * leave this out and the class will specialize.
			 *
			 * @param name - Must be unique to the shader, don't use var### because it is the
			 * procedurally generated form that the nodes use.
			 * @param value - A shared pointer to a statically sized array or value.
			 */
			template <class R, typename T, unsigned size>
			class ArrayAttribute : public IAttribute<R> {
			private:
				std::array<T, size> value;
			public:

				Output<R, T> out;

				ArrayAttribute(const std::string& name, ShaderScope s, const std::array<T, size>& value) : value(value) {
					out.name = name;
				}
				ArrayAttribute(std::string&& name, ShaderScope s, std::array<T,size>&& value) : value(value) {
					out.name = name;
				}
				virtual ~ArrayAttribute(){}

				ArrayAttribute(const ArrayAttribute& a){ //Copy
					copy(a);
				}

				ArrayAttribute(ArrayAttribute&& a) : IShaderNode<R>(a) { //Move
					value = a.value;
				}

				ArrayAttribute& copy(const ArrayAttribute& a){
					IShaderNode<R>::IShaderNode(a);
					value = a.value;
					return *this;
				}

				ArrayAttribute& operator=(const ArrayAttribute& a){
					return copy(a);
				}

				//This must be defined with a partial specialization in each renderer R
				std::string getStatic() const override {
					std::string ret;
					switch(scope){
						case PerPrimitive:
						ret = "uniform";
						break;
						case PerVertex:
						ret = "attribute";
						break;
					}
					ret += " " + Utility<R, T>::type + " " + name + "[" + size + "];";
					return ret;
				}

				virtual void setValue(const std::array<T, size>& value){
					dirty = true;
					this->value = value;
				}
				virtual void setValue(std::array<T, size>&& value){
					dirty = true;
					this->value = value;
				}

			};

			template <class R, typename T>
			class Attribute : public IAttribute<R> {
			private:
				T value;
			public:

				Output<R, T> out;

				Attribute(const std::string& name, ShaderScope s, const T& value) : value(value) {
					out.name = name;
				}
				Attribute(std::string&& name, ShaderScope s, T&& value) : value(value) {
					out.name = name;
				}
				virtual ~Attribute(){}

				Attribute(const Attribute& a) : IShaderNode<R>(a) { //Copy
					value = a.value;
				}

				Attribute(Attribute&& a) : IShaderNode<R>(a) { //Move
					value = a.value;
				}

				Attribute& copy(const Attribute& a){
					IShaderNode<R>::operator=(a);
					value = a.value;
					return *this;
				}

				Attribute& operator=(const Attribute& a){
					return copy(a);
				}

				//This must be defined with a partial specialization in each renderer R
				std::string getStatic() const override {
					std::string ret;
					switch(scope){
						case PerPrimitive:
						ret = "uniform";
						break;
						case PerVertex:
						ret = "attribute";
						break;
					}
					ret += " " + Utility<R, T>::type + " " + name + ";";
					return ret;
				}

				virtual void setValue(const T& val){
					value = val;
				}

				virtual void setValue(T&& val){
					value = val;
				}

			};

		}
	}
}

//Circular
#include "Shader/ShaderNode.hpp"


#endif /* INCLUDE_SHADER_SHADERATTRIBUTE_HPP_ */
