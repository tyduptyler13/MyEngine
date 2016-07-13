#ifndef MYUPLAY_MYENGINE_SHADER
#define MYUPLAY_MYENGINE_SHADER

#include <vector>
#include <memory>

#include "Shader/ShaderComponent.hpp"

namespace MyUPlay {
namespace MyEngine {
namespace Shader {

template <typename R>
class Shader {

private:

	bool dirty = true;
	ShaderComponent<R> vertexShader;
	ShaderComponent<R> fragmentShader;

	void onChangeFunc(){
		dirty = true;
	}

public:

	Shader(){

	}
	virtual ~Shader(){}


	bool isDirty(){
		return dirty;
	}

	virtual void bind() = 0;

};

}
}
}


#endif
