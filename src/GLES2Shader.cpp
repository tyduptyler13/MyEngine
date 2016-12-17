
#include <exception>
#include <unordered_set>

#include "../include/Shader/GLES2Shader.hpp"
#include "Log.hpp"

#include "glsl/glsl_optimizer.h"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace MyUPlay::MyEngine::Shader;

static Log shaderLog("ShaderGLES2");

static string fetchCode(IShaderNode* root){

	string staticCode = "//THIS IS GENERATED CODE!!!\n\n" + root->getStatic();
	string mainCode = "//This is generated code, do not try to directly modify!\n\n"
			"void main(){\n";

	std::unordered_set<Math::UUID> visited;

	IShaderNode::ShaderTraverser trav = [&](shared_ptr<IShaderNode> node){
		if (node == nullptr) return; //Some inputs don't have nodes attached!
		if (visited.find(node->uuid) == visited.end()){ //Have we seen this node before?
			visited.insert(node->uuid);
			staticCode += node->getStatic(); //Foward recursion (breadth first)
			node->traverseChildren(trav);
			mainCode += node->getInstance(); //Tail recursion (depth first)
		}
	};

	root->traverseChildren(trav);

	mainCode += root->getInstance();

	mainCode += "\n}\n//This is generated code, do not try to directly modify!\n";

	return staticCode + mainCode;

}

static inline void reportError(GLuint shader) {
	GLint maxLength;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	string infoLog(maxLength, ' ');
	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

	shaderLog.log("GL Log from previous error: \n" + infoLog);
}

void GLES2Shader::compile() {

	static glslopt_ctx *ctx = glslopt_initialize(kGlslTargetOpenGLES20); //FIXME, This should be put somewhere more global and should be destructed.

	pos.clear(); //Want to forget old positions.

	string vertexCode = fetchCode(getVertexRoot());
	shaderLog.debug("Vertex shader:\n" + vertexCode);
	string fragmentCode = fetchCode(getFragmentRoot());
	shaderLog.debug("Fragment shader:\n" + fragmentCode);

	glslopt_shader* shader = glslopt_optimize(ctx, kGlslOptShaderVertex, vertexCode.c_str(), 0);
	if (glslopt_get_status(shader)){
		vertexCode = glslopt_get_output(shader);
		shaderLog.debug("Optimized vertex shader:\n" + vertexCode);
	} else {
		shaderLog.error(string("Failed to optimize vertex shader!\n") + glslopt_get_log(shader));
	}
	glslopt_shader_delete(shader);
	shader = glslopt_optimize(ctx, kGlslOptShaderFragment, fragmentCode.c_str(), 0);
	if (glslopt_get_status(shader)){
		fragmentCode = glslopt_get_output(shader);
		shaderLog.debug("Optimized fragment shader:\n" + fragmentCode);
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

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	cp = fragmentCode.c_str(); //Getting an lvalue.
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

void GLES2Vertex::prepare(Camera<float>* camera, Mesh<float>* object, const std::vector<Light<float>*>& lights) {

	if (shader->dirty) shader->compile(); //Check if the shader needs an update

	shader->bind(); //The shader needs to already be bound.

	IGeometry<float>* geometry = object->geometry.get();

	//Working on adding VAO support, might have to wait for GLES3 Renderer
	/*if (geometry->vertexObject == 0) {
		glGenVertexArraysOES(1, &geometry->vertexObject);
	}

	glBindVertexArrayOES(geometry->vertexObject);*/

	if (geometry->indicesNeedUpdate) {

		if (geometry->indexBuffer == 0) {
			glGenBuffers(1, &geometry->indexBuffer); //TODO Move this value somewhere we can manage its memory
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indexBuffer);
		//TODO look into if we should always static draw
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * geometry->getIndices().size(), geometry->getIndices().data(), GL_STATIC_DRAW);

		geometry->indicesNeedUpdate = false;

	}

	if (geometry->verticesNeedUpdate) {

		if (geometry->vertexBuffer == 0){
			glGenBuffers(1, &geometry->vertexBuffer); //TODO Move this value somewhere we can manage its memory
		}

		glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
		//TODO look into if we should always static draw
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geometry->getVertices().size(), geometry->getVertices().data(), GL_STATIC_DRAW);

		geometry->verticesNeedUpdate = false;

	}

	if (geometry->normalsNeedUpdate) {
		if (geometry->normalBuffer == 0){
			glGenBuffers(1, &geometry->normalBuffer); //TODO Move this value somewhere we can manage its memory
		}

		glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
		//TODO look into if we should always static draw
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geometry->getNormals().size(), geometry->getNormals().data(), GL_STATIC_DRAW);

		geometry->normalsNeedUpdate = false;
	}

	GLint posLoc = shader->getAttribLoc("position");

	if (posLoc != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
		glEnableVertexAttribArray(posLoc);
		glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	GLint normLoc = shader->getAttribLoc("normal");

	if (normLoc != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
		glEnableVertexAttribArray(normLoc);
		glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	GLint cameraLoc = shader->getUniformLoc("cameraPosition");

	if (cameraLoc != -1) {
		glUniform3f(cameraLoc, camera->position.x, camera->position.y, camera->position.z);
	}

	GLint projectionMatrixLoc = shader->getUniformLoc("projectionMatrix");

	if (projectionMatrixLoc != -1) {
		glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, camera->projectionMatrix.elements.data());
	}

	GLint viewMatrixLoc = shader->getUniformLoc("viewMatrix");

	if (viewMatrixLoc != -1) {
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, camera->matrixWorldInverse.elements.data());
	}

	GLint modelMatrixLoc = shader->getUniformLoc("modelMatrix");

	if (modelMatrixLoc != -1) {
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, object->matrixWorld.elements.data());
	}

	GLint modelViewLoc = shader->getUniformLoc("modelView");

	if (modelViewLoc != -1) {
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, object->modelViewMatrix.elements.data());
	}

}

void GLES2Fragment::prepare(Camera<float>*, Mesh<float>*, const std::vector<Light<float>*>&) {

	//TODO Handle custom attributes, otherwise we have 0 things to add by default.
	//FIXME Custom attribute handling should be handled by a base class. (Is this the base class?)

}



