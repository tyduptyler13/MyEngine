#define GLFW_INCLUDE_COREARB
#define GLFW_INCLUDE_GLEXT

#include "GL32Renderer.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

static Log logger("GL32Renderer");

GL32Renderer::GL32Renderer(unsigned antialias, GLFWmonitor* monitor, GL32Renderer* share) {

	logger.log("Preparing GL32Renderer");

	std::vector<std::pair<int, int>> hints = {{
			std::make_pair(GLFW_CLIENT_API, GLFW_OPENGL_API), //Hard constraint.
			std::make_pair(GLFW_CONTEXT_VERSION_MAJOR, 3),
			std::make_pair(GLFW_CONTEXT_VERSION_MINOR, 2),
			std::make_pair(GLFW_SAMPLES, antialias)
	}};

	this->glfw = new GLFWManager(hints, 800, 600, "MyEngine", monitor, share ? share->glfw : nullptr);

	auto lock = GLFWManager::getLock();

	this->glfw->makeContextCurrent();

	try {
		std::string version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		logger.log("GL VERSION: " + version);
		std::string renderer(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		logger.log("GL RENDERER: " + renderer);
	} catch (...) {
		logger.log("GL Error" + std::to_string(glGetError()));
		throw runtime_error("Failed to call basic Opengl function. Context problems!");
	}

	//Every window instance needs to be added.

	this->glfw->onResize([this](int width, int height){
		this->windowWidth = width;
		this->windowHeight = height;
	});

	onResize([](int width, int height){
		glViewport(0, 0, width, height);
	});

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

}

GL32Renderer::~GL32Renderer(){
	delete glfw;
}
