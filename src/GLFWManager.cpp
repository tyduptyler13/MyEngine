#include <unordered_map>

#include "GLFWManager.hpp"

using namespace MyUPlay::MyEngine;
using namespace std;

/**
 * Due to the nature of GLFW callbacks, we use a static struct to receive the events but
 * then we have to find the actual function that wants to receive the call, thus the following
 * functions will wrap and handle things like we want them to be.
 */
template <int, typename... Args> //The int allows you to differentiate static "instances".
struct StaticInstanceEventMapper {

	typedef std::function<void(Args...)> RemapperFunction;
	typedef std::unordered_map<GLFWwindow*, std::vector<RemapperFunction>> RemapperContainer;

	static RemapperContainer functionRemapper;

	static void handleEvent(GLFWwindow* window, Args... args) { //GLFW calls this
		for (auto func : functionRemapper[window]){
			func(args...);
		}
	}
	static void addHandler(GLFWwindow* window, RemapperFunction func){ //Adds a hook for when glfw calls us.
		functionRemapper[window].push_back(func);
	}
};

typedef StaticInstanceEventMapper<0, int, int> WindowSizer; //Handles window resizing
typedef StaticInstanceEventMapper<1, int, int> FrameSizer; //Handles framebuffer resizing

template<>
WindowSizer::RemapperContainer WindowSizer::functionRemapper = WindowSizer::RemapperContainer();
template <>
FrameSizer::RemapperContainer FrameSizer::functionRemapper = FrameSizer::RemapperContainer();

static WindowSizer windowSizer;
static FrameSizer frameSizer;

unsigned GLFWManager::instances = 0;
Log GLFWManager::glfwLog("GLFW");
std::recursive_mutex GLFWManager::lock;

/**
 * Add a function to handle when the window gets resized.
 */
void GLFWManager::onResize(std::function<void(int, int)> func) const {
	windowSizer.addHandler(window, func);
}
/**
 * Add a function to handle when the framebuffer gets resized.
 */
void GLFWManager::onFrameResize(std::function<void(int, int)> func) const {
	frameSizer.addHandler(window, func);
}

void GLFWManager::init() {
	glfwSetWindowSizeCallback(window, windowSizer.handleEvent);
	glfwSetFramebufferSizeCallback(window, frameSizer.handleEvent);
}

