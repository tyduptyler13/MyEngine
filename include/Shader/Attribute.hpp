/*
 * ShaderAttribute.hpp
 *
 *  Created on: Jul 13, 2016
 *      Author: tyler
 */

#ifndef MyUPlay_MyEngine_Attribute
#define MyUPlay_MyEngine_Attribute

#include "Math.hpp"

namespace MyUPlay{
namespace MyEngine {
namespace Shader {

class IAttribute {
public:
	std::string uuid = Math::generateUUID();
	std::string name;

	virtual ~IAttribute(){}

	template <typename T>
	virtual void set(T* value, unsigned size = 0) = 0;

	virtual std::string get() = 0;
}

/**
 * Renderer followed by the type the class holds.
 * This will be automatically converted thanks to specializations in each render class
 */
template <typename R, typename T>
class Attribute : IAttribute {
public:
	T* value = NULL;
	unsigned size = 0; //Set this to a non zero value an the shader will use an array implementation instead. (1 is a single point array)
	std::unique_ptr<std::function<void(T*)> > update = NULL; //Override this if you want the value updated every frame
	//If this is null, it will always use whatever is in value.

	static const char* type;

	Attribute(const std::string& name, T* value = NULL) : name(name), value(value) {}
	Attribute(std::string&& name, T* value = NULL) : name(name), value(value) {}
	~Attribute(){
		if (value == NULL) return;
		if (size == 0){
			delete value;
		} else {
			delete[] value;
		}
	}

	Attribute(const Attribute& a){ //Copy
		copy(a);
	}

	Attribute(Attribute&& a){ //Move
		value = a.value;
		a.value = NULL; //Prevent the other from deleting our shit.
		name = a.name;
		size = a.size;
	}

	Attribute& copy(const Attribute& a){
		value = new T[size];
		for (unsigned i = 0; i < size; ++i){
			value[i] = a.value[i];
		}
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
	void set(T* value, unsigned size = 0) override {
		if (value != NULL){
			if (size == 0){
				delete value;
			} else {
				delete[] value;
			}
		}
		this->value = value;
		this->size = size;
	}

	std::string get();


};

}
}
}


#endif /* INCLUDE_SHADER_SHADERATTRIBUTE_HPP_ */
