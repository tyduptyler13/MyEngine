#ifndef MYUPLAY_MYENGINE_SHADERCOMPONENT
#define MYUPLAY_MYENGINE_SHADERCOMPONENT

#include <vector>
#include <string>
#include <map>
#include <stdexcept>

#include "Math.hpp"

namespace MyUPlay {
namespace MyEngine {
namespace Shader {

class Connector {
public:
	weak_ptr<ShaderComponent> component;
	std::string type;
	std::string name;
};

class Output;

/**
 * A Connector graph is marched from output and follows dependent (input) functions. This removes
 * dead code naturally by ref counting only Input sources. This also prevents circular dependencies.
 */
class Input : Connector {
public:
	/**
	 * If a ShaderComponent can function without the input either by default values
	 * or because there are multiple definitions of the function then set this to false.
	 */
	bool required = true;
	shared_ptr<Output> source;
};

class Output : Connector {
public:
	std::vector<std::weak_ptr<Input>> dest;
};

class ShaderComponent {

public:

	const Math::UUID = Math::GenerateUUID();

	const std::string name; //This should exactly match the functions name in the code. The code may not compile otherwise.
	/**
	 * Only add an input here if code MUST set its value before the shader is run.
	 * If it is a varying type variable then don't put it here, glsl will handle it.
	 */
	const std::map<std::string, IOin> inputs; //Input variables.
	const std::map<std::string, IOout> outputs; //Output of this function. (only used in node graph)

	const std::string code;

	virtual ~ShaderComponent(){}
	ShaderComponent(const std::string& name, const std::vector<Input>& inputs, const std::vector<Output>& outputs, const std::string& code)
	: name(name), inputs(inputs), outputs(outputs), code(code) {}
	ShaderComponent(std::string&& name, std::vector<IO>&& inputs, std::vector<IO>&& outputs, std::string&& code)
	: name(name), inputs(inputs), outputs(outputs), code(code) {}
	ShaderComponent(ShaderComponent&& c) : name(c.name), inputs(c.inputs), outputs(c.outputs), code(c.code) {}

};


}
}
}


#endif
