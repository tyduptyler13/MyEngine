#ifndef MYUPLAY_MYENGINE_SHADERCOMPONENT
#define MYUPLAY_MYENGINE_SHADERCOMPONENT

#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>

namespace MyUPlay {
namespace MyEngine {
namespace Shader {

struct IO {
	std::string type;
	std::string name;
};

class ShaderComponent {

public:

	std::string name;
	virtual ~ShaderComponent(){}

	std::vector<std::string> extensions; //Required extensions
	/**
	 * Only add an input here if code MUST set its value before the shader is run.
	 * If it is a varying type variable then don't put it here, glsl will handle it.
	 */
	std::vector<std::unique_ptr<IO> inputs; //Input variables.
	std::vector<std::unique_ptr<IO> outputs; //Output of this function. (only used in node graph)

	std::string code;

	virtual void compile();

};


}
}
}


#endif
