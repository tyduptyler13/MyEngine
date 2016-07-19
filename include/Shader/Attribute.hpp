/*
 * ShaderAttribute.hpp
 *
 *  Created on: Jul 13, 2016
 *      Author: tyler
 */

#ifndef MyUPlay_MyEngine_Attribute
#define MyUPlay_MyEngine_Attribute

#include <array>

#include "Math.hpp"

namespace MyUPlay{
namespace MyEngine {
namespace Shader {

class IAttribute {
public:
	std::string uuid = Math::generateUUID();
	std::string name;

	virtual ~IAttribute(){}

	template <typename T, unsigned size = 0>
	virtual void set(const std::array<T, size>0?size:1>& value) = 0;

	virtual std::string get() = 0;
}

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
	std::array<T, size > 0 ? size : 1> value; //Always will have at least one element.
public:
	std::unique_ptr<std::function<std::array<T, size>0?size:1>()> update = NULL; //Override this if you want the value updated every frame
	//If this is null, it will always use whatever is in value, you can use set to change it.

	static const char* type;

	Attribute(const std::string& name, const std::array<T, size>0 ?size:1>& value) : name(name), value(value) {}
	Attribute(std::string&& name, std::array<T, size>0?size:1>&& value) : name(name), value(value) {}
	~Attribute(){}

	Attribute(const Attribute& a){ //Copy
		copy(a);
	}

	Attribute(Attribute&& a){ //Move
		value = a.value;
		name = a.name;
		size = a.size;
	}

	Attribute& copy(const Attribute& a){
		value = a.value;
		name = a.name;
		size = a.size;
		return *this;
	}

	Attribute& operator=(const Attribute& a){
		return copy(a);
	}

	/**
	 * Expect that the value you pass in will be managed from now on
	 * If this object is deleted or set is called again, the existing memory will be freed.
	 */
	void set(const array<T, size>0?size:1>& value) override {
		this->value = value;
		this->size = size;
	}

	//This must be defined with a partial specialization in each renderer R
	std::string get() override;



};

}
}
}


#endif /* INCLUDE_SHADER_SHADERATTRIBUTE_HPP_ */
