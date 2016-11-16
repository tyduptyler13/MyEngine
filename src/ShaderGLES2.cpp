
#include <exception>
#include <unordered_set>

#include "Shader/ShaderGLES2.hpp"
#include "Log.hpp"

#include "glsl/glsl_optimizer.h"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;

static string fetchCode(IShaderNode& root){

	string staticCode;
	string mainCode = "//This is generated code, do not try to directly modify!\n\n"
			"void main(){\n";

	std::unordered_set<Math::UUID> visited;

	IShaderNode::ShaderTraverser trav = [&](shared_ptr<IShaderNode> node){
		if (visited.find(node->uuid) == visited.end()){ //Have we seen this node before?
			visited.insert(node->uuid);
			staticCode += node->getStatic(); //Foward recursion (breadth first)
			node->traverseChildren(trav);
			mainCode += node->getInstance(); //Tail recursion (depth first)
		}
	};

	root.traverseChildren(trav);

	mainCode += "\n}\n//This is generated code, do not try to directly modify!";

	return staticCode + mainCode;

}

static Log shaderLog("GLES2ShaderCompiler");

static inline void reportError(GLuint shader) {
	GLint maxLength;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	string infoLog(maxLength, ' ');
	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

	shaderLog.log("GL Log from previous error: \n" + infoLog);
}

void GLES2ForwardShader::compile() {

	static glslopt_ctx *ctx = glslopt_initialize(kGlslTargetOpenGLES20); //FIXME, This should be put somewhere more global and should be destructed.

	pos.clear(); //Want to forget old positions.

	string vertexCode = fetchCode(*vertexShaderRoot);
	shaderLog.log("Vertex shader:\n" + vertexCode);
	string fragmentCode = fetchCode(*fragmentShaderRoot);
	shaderLog.log("Fragment shader:\n" + fragmentCode);

	glslopt_shader *shader = glslopt_optimize(ctx, kGlslOptShaderVertex, vertexCode.c_str(), 0);
	if (glslopt_get_status(shader)){
		vertexCode = glslopt_get_output(shader);
		shaderLog.log("Optimized vertex shader:\n" + vertexCode);
	} else {
		shaderLog.error(string("Failed to optimize vertex shader!\n") + glslopt_get_log(shader));
	}
	glslopt_shader_delete(shader);
	shader = glslopt_optimize(ctx, kGlslOptShaderFragment, fragmentCode.c_str(), 0);
	if (glslopt_get_status(shader)){
		fragmentCode = glslopt_get_output(shader);
		shaderLog.log("Optimized fragment shader:\n" + fragmentCode);
	} else {
		shaderLog.error(string("Failed to optimize fragment shader!\n") + glslopt_get_log(shader));
	}
	glslopt_shader_delete(shader);

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	const char* cp = vertexCode.c_str(); //Getting an lvalue.
	glShaderSource(vertShader, 1, &cp, 0);

	glCompileShader(vertShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE){
		shaderLog.error("The vertex shader failed to compile!");

		reportError(vertShader);
		glDeleteShader(vertShader);
		throw runtime_error("Failed to compile a shader! See log for details.");
	}

	GLuint fragmentShader = glCreateShader(GL_VERTEX_SHADER);
	cp = vertexCode.c_str(); //Getting an lvalue.
	glShaderSource(fragmentShader, 1, &cp, 0);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE){
		shaderLog.error("The fragment shader failed to compile!");

		reportError(fragmentShader);
		glDeleteShader(fragmentShader);
		throw runtime_error("Failed to compile a shader! See log for details.");
	}

	if (program != 0) glDeleteProgram(program); //We already have a program, so we delete it on the gpu before creating a new one.
	program = glCreateProgram();

	glAttachShader(program, vertShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE){
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		string infoLog(maxLength, ' ');
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		program = 0;

		shaderLog.error("Failed to compile program!");

		shaderLog.log("GL log from previous error: \n" + infoLog);

		throw runtime_error("Failed to link a shader! See log for details.");

	}

	//We don't need these sub components anymore, the program has it all.
	glDetachShader(program, vertShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertShader); //This allows the shaders to be deleted if the program is deleted.
	glDeleteShader(fragmentShader);

	dirty = false; //This call will clear the dirty flag.

}

void GLES2ForwardShader::prepare(Camera<float>* camera, Mesh<float>* object, const vector<Light<float>*>& lights){
	this->vertexShaderRoot->prepare(camera, object, lights);
	this->fragmentShaderRoot->prepare(camera, object, lights);
}

void GLES2ForwardShader::render(int group){
	//TODO
}

void GLES2Vertex::prepare(Camera<float>* camera, Mesh<float>* object, const std::vector<Light<float>*>& lights) {

	shader->bind(); //The shader needs to already be bound.

	GLint posLoc = shader->getAttribLoc("position");

	if (posLoc == 0) {
		shaderLog.error("Failed to get position of required 'position' attribute!");
		throw runtime_error("Couldn't get position of attribute. No progress could be made.");
	}

	GLint normLoc = shader->getAttribLoc("normal");

	if (normLoc == 0) {
		shaderLog.error("Failed to get position of required 'normal' attribute!");
		throw runtime_error("Couldn't get position of attribute. No progress could be made.");
	}

}

void GLES2Fragment::prepare(Camera<float>* camera, Mesh<float>* object, const std::vector<Light<float>*>& lights) {

	//TODO Handle custom attributes, otherwise we have 0 things to add by default.
	//FIXME Custom attribute handling should be handled by a base class. (Is this the base class?)

}



