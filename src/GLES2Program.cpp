#include "GLES2Program.hpp"

#include <sstream>
#include <regex>

using namespace std;
using namespace MyUPlay::MyEngine;

string GLES2Program::generateDefines(const vector<pair<string, string> >& defines){

	ostringstream chunks;

	for (const pair<string, string>& name : defines){

		if (name.second == "") continue;

		chunks << "#define " << name.first << " " << name.second << "\n";

	}

	return chunks.str();

}

vector<pair<string, GLint> > GLES2Program::fetchUniformLocations(GLuint program){

	vector<pair<string, GLint> > uniforms;

	GLint uniformCount;

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);

	const regex filter("\\\\[0\\\\]$");

	GLchar* name = new GLchar[255];
	const GLsizei bufSize = 255;
	GLsizei length;
	GLint size;
	GLenum type;

	for (GLint i = 0; i < uniformCount; ++i){	

		glGetActiveUniform(program, i, bufSize, &length, &size, &type, name);
		GLint location = glGetUniformLocation(program, name);

		uniforms.push_back(make_pair(name, location));

		if (regex_search(name, filter)){

			regex_replace(name, filter, "");

			uniforms.push_back(make_pair(name, location));

		}

	}

	delete[] name;

	return uniforms;

}

vector<pair<string, GLint> > GLES2Program::fetchAttributeLocations(GLuint program){

	vector<pair<string, GLint> > attributes;

	GLint attributeCount;

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributeCount);

	GLchar* name = new GLchar[255];
	const GLsizei bufSize = 255;
	GLsizei length;
	GLint size;
	GLenum type;

	for (GLint i = 0; i < attributeCount; ++i){

		glGetActiveAttrib(program, i, bufSize, &length, &size, &type, name);

		GLint location = glGetAttribLocation(program, name);

		attributes.push_back(make_pair(name, location));

	}

	delete[] name;

	return attributes;

}

//TODO

