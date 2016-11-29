#include "Shader/ShaderNode.hpp"

using namespace MyUPlay::MyEngine;
using namespace std;

static volatile int varCounter = 1;

string Shader::generateUniqueName(){
	return "var" + std::to_string(varCounter++);
}
