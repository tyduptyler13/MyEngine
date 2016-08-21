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

#include "Math.hpp"
#include "Shader/ShaderUtil.hpp"

namespace MyUPlay{
	namespace MyEngine {
		namespace Shader {

			class IAttribute {
			public:
				enum Scope {
					Const, //Compile time
					PerFrame, //Uniform
					PerPrimative, //Attribute
					PerVertex //Varying
				};
			protected:
				IAttribute(std::string name, Scope s) : name(name), scope(s) {}
			public:
				Scope scope;

				std::string uuid = Math::generateUUID();
				std::string name;

				virtual ~IAttribute(){}

				virtual std::string text() const = 0; //Generates the string for code
				virtual void push() = 0; //Pushes the native values to the gpu
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
			 */
			template <typename R, typename T, unsigned size = 0>
			class Attribute : IAttribute {
			private:
				std::array<T, (size>0?size:1)> value; //Always will have at least one element.
			public:
				std::unique_ptr<std::function<std::array<T,(size>0?size:1)>()>> update = NULL; //Override this if you want the value updated every frame
				//If this is null, it will always use whatever is in value, you can use set to change it.

				static const char* type;

				Attribute(const std::string& name, const std::array<T, (size>0?size:1)>& value, Scope s) : IAttribute(name, s), value(value) {}
				Attribute(std::string&& name, std::array<T,(size>0?size:1)>&& value, Scope s) : IAttribute(name, s), value(value) {}
				~Attribute(){}

				Attribute(const Attribute& a){ //Copy
					copy(a);
				}

				Attribute(Attribute&& a){ //Move
					value = a.value;
					name = a.name;
				}

				Attribute& copy(const Attribute& a){
					value = a.value;
					name = a.name;
					return *this;
				}

				Attribute& operator=(const Attribute& a){
					return copy(a);
				}

				/**
				 * Expect that the value you pass in will be managed from now on
				 * If this object is deleted or set is called again, the existing memory will be freed.
				 */
				void set(const std::array<T,(size>0?size:1)>& value) {
					this->value = value;
				}

				//This must be defined with a partial specialization in each renderer R
				std::string text() const override;
				void push() override;

			};

			template <typename R, typename T, unsigned size>
			const char* Attribute<R, T, size>::type = Utility<R, T>::Type;

			template <typename T, unsigned size>
			std::string Attribute<GLES2Renderer, T, size>::text() const {
				std::string ret;
				switch(scope){
					case Const:
					ret = "const";
					break;
					case PerFrame:
					ret = "uniform";
					break;
					case PerPrimative:
					ret = "attribute";
					break;
					case PerVertex:
					ret = "varying";
					break;
				}
				ret += " " + Attribute::type;
				if (size > 1){
					ret += "[" + size + "]";
				}
				ret += " " + name + ";";
				return ret;
			}

		}
	}
}


#endif /* INCLUDE_SHADER_SHADERATTRIBUTE_HPP_ */
