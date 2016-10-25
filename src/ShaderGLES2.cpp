
#include <exception>

#include "Shader/ShaderGLES2.hpp"
#include "Log.hpp"

#include "glsl/glsl_optimizer.h"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;

void GLES2Shader::bind(){
	glUseProgram(program);
}

static string fetchCode(IShaderNode& root){
	vector<shared_ptr<IShaderNode>> nodes; //For doing bredth first searches

		//Seed the initial variables.
		root.traverseChildren([&](shared_ptr<IShaderNode> node){
			nodes.push_back(node);
		});

		for (unsigned i = 0; i < nodes.size(); ++i){ //nodes size will change at almost every iteration, we are following its depth

			shared_ptr<IShaderNode>& node = nodes[i];


			node->traverseChildren([&](shared_ptr<IShaderNode> node){
				nodes.push_back(node);
			});

		} //This has constructed a list that guarantees breadth first.

		string code;

		for (int i = nodes.size() - 1; i >= 0; --i){
			code += nodes[i]->getStatic();
		}

		code += "\n\n"
				"void main(){\n";

		for (int i = nodes.size() -1; i >= 0; --i){
			code += nodes[i]->getInstance();
		}

		code += "\n}\n//This is generated code, do not try to directly modify!";

		return code;

}

static Log shaderLog("GLES2ShaderCompiler");

static inline void reportError(GLuint shader) {
	GLint maxLength;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	string infoLog(maxLength, ' ');
	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

	shaderLog.log("GL Log from previous error: \n" + infoLog);
}

void ForwardShaderGLES2::compile() {

	static glslopt_ctx *ctx = glslopt_initialize(kGlslTargetOpenGLES20); //FIXME, This should be put somewhere more global and should be destructed.

	string vertexCode = fetchCode(*vertexShader);
	shaderLog.log("Vertex shader:\n" + vertexCode);
	string fragmentCode = fetchCode(*fragmentShader);
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
	glDeleteShader(vertShader);
	glDeleteShader(fragmentShader);

}
