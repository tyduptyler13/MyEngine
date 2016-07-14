#ifndef MYUPLAY_MYENGINE_SHADER
#define MYUPLAY_MYENGINE_SHADER

#include <vector>
#include <memory>

#include "Shader/Attribute.hpp"
#include "Shader/ShaderComponent.hpp"

namespace MyUPlay {
namespace MyEngine {
namespace Shader {

class Shader {

private:

	bool dirty = true;
	std::vector<IAttribute> attributes;
	ShaderComponent vertexShader;
	ShaderComponent fragmentShader;

	void onChangeFunc(){
		dirty = true;
	}

public:

	bool optimize = true;
	bool debug = false;

	Shader(){

	}
	virtual ~Shader(){}

	bool isDirty(){
		return dirty;
	}

	template <typename R>
	void compile();

	template <typename R>
	void bind();

};

}
}
}


#endif
