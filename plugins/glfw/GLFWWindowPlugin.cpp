#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include "Plugin.hpp"
#include "DefaultRenderer.hpp"
#include "BasicWindowEventHandler.hpp"

using namespace MyEngine;
using namespace MyEngine::DefaultRenderer;

namespace MyEngine::GLFW {
	class GLFWWindow final : public BasicWindowEventHandler {

		static std::unordered_map<GLFWwindow*, GLFWWindow*> eventContextMap;

		static const std::unordered_map<int, KeyboardKey> glfwKeyMap;

		GLFWwindow* window;

		struct {
			int x = 0, y = 0;
			int width = 800, height = 600;
		} previous;

	public:
		explicit GLFWWindow(const InitHints& initHints) {

			if (initHints.graphicsApiHint == GraphicsAPIHint::OPENGL) {
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			} else if (initHints.graphicsApiHint == GraphicsAPIHint::GLES) {
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
			} else {
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}

			window = glfwCreateWindow(static_cast<int>(initHints.widthHint), static_cast<int>(initHints.heightHint),
			                          initHints.windowTitleHint.c_str(), nullptr, nullptr);

			if (window == nullptr) {
				throw std::runtime_error("Failed to create the glfw window");
			}

			eventContextMap.emplace(window, this);

			glfwSetWindowSizeLimits(window,
			                        initHints.minWidthHint == -1 ? GLFW_DONT_CARE : initHints.minWidthHint,
			                        initHints.minHeightHint == -1 ? GLFW_DONT_CARE : initHints.minHeightHint,
			                        initHints.maxWidthHint == -1 ? GLFW_DONT_CARE : initHints.maxWidthHint,
			                        initHints.maxHeightHint == -1 ? GLFW_DONT_CARE : initHints.maxHeightHint
			);

			glfwSetKeyCallback(window, onKeyEvent);
		}

		~GLFWWindow() {
			eventContextMap.erase(window);
			glfwDestroyWindow(window);
		}

	private:

		static void onKeyEvent(GLFWwindow* window, int glfwKey, int scancode, int action, int mods) {
			auto windowIt = eventContextMap.find(window);
			if (windowIt == eventContextMap.end()) {
				throw std::runtime_error("The window is no longer bound in the context map but events are still firing!");
			}

			auto keyIt = glfwKeyMap.find(glfwKey);
			KeyboardKey key = keyIt != glfwKeyMap.end() ? keyIt->second : KeyboardKey::KEY_UNKNOWN;

			EventType type;
			switch (action) {
				case GLFW_RELEASE:
					type = EventType::Released;
					break;
				case GLFW_PRESS:
					type = EventType::Pressed;
					break;
				case GLFW_REPEAT:
					type = EventType::Repeat;
					break;
				default:
					type = EventType::None;
			}

			windowIt->second->triggerKeyboardHandlers(key, type);
		}

		Window* show() override {
			glfwShowWindow(window);
			return this;
		}

		Window* hide() override {
			glfwHideWindow(window);
			return this;
		}

		Window* setPos(unsigned int x, unsigned int y) override {
			glfwSetWindowPos(window, x, y);
			return this;
		}

		Window* resize(unsigned int width, unsigned int height) override {
			glfwSetWindowSize(window, static_cast<int>(width), static_cast<int>(height));
			return this;
		}

		Window* setTitle(const std::string& title) override {
			glfwSetWindowTitle(window, title.c_str());
			return this;
		}

		Window* toggleBorder() override {
			glfwSetWindowAttrib(window, GLFW_DECORATED,
			                    glfwGetWindowAttrib(window, GLFW_DECORATED) == GLFW_TRUE ? GLFW_TRUE : GLFW_FALSE);
			return this;
		}

		Window* toggleFullscreen() override {
			if (glfwGetWindowMonitor(window) == nullptr) {
				glfwGetWindowPos(window, &previous.x, &previous.y);
				glfwGetWindowSize(window, &previous.width, &previous.height);
				GLFWmonitor* monitor = getCurrentMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			} else {
				glfwSetWindowMonitor(window, nullptr, previous.x, previous.y, previous.width, previous.height, 0);
			}

			return this;
		}

		bool hasBorder() override {
			return glfwGetWindowAttrib(window, GLFW_DECORATED) == GLFW_TRUE;
		}

		bool isFullScreen() override {
			return glfwGetWindowMonitor(window) != nullptr;
		}

		Window* setMouseLock() override {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			return this;
		}

		Window* processEvents() override {
			glfwPollEvents();
			return this;
		}

		Window* swapBuffers() override {
			return Window::swapBuffers();
		}

		static int mini(int x, int y) {
			return x < y ? x : y;
		}

		static int maxi(int x, int y) {
			return x > y ? x : y;
		}

		// https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
		GLFWmonitor* getCurrentMonitor() {
			int nmonitors, i;
			int wx, wy, ww, wh;
			int mx, my, mw, mh;
			int overlap, bestoverlap;
			GLFWmonitor* bestmonitor;
			GLFWmonitor** monitors;
			const GLFWvidmode* mode;

			bestoverlap = 0;
			bestmonitor = NULL;

			glfwGetWindowPos(window, &wx, &wy);
			glfwGetWindowSize(window, &ww, &wh);
			monitors = glfwGetMonitors(&nmonitors);

			for (i = 0; i < nmonitors; i++) {
				mode = glfwGetVideoMode(monitors[i]);
				glfwGetMonitorPos(monitors[i], &mx, &my);
				mw = mode->width;
				mh = mode->height;

				overlap =
						maxi(0, mini(wx + ww, mx + mw) - maxi(wx, mx)) *
						maxi(0, mini(wy + wh, my + mh) - maxi(wy, my));

				if (bestoverlap < overlap) {
					bestoverlap = overlap;
					bestmonitor = monitors[i];
				}
			}

			return bestmonitor;
		}


	};

	struct GLFWWindowPlugin final : public Plugin {
		GLFWWindowPlugin() = default;

		~GLFWWindowPlugin() override = default;

		static std::unique_ptr<Window> createGLFWWindow(const InitHints& initHints) {
			return std::make_unique<GLFWWindow>(initHints);
		}

		WindowFactoriesContainer::const_iterator it;

		void onLoad() override {
			it = DefaultRenderer::dynamicWindowFactories.emplace(WindowSystemHint::GLFW, GLFWWindowPlugin::createGLFWWindow);
		}

		void onUnload() override {
			DefaultRenderer::dynamicWindowFactories.erase(it);
		}
	};
}

std::unordered_map<GLFWwindow*, MyEngine::GLFW::GLFWWindow*> MyEngine::GLFW::GLFWWindow::eventContextMap;

const std::unordered_map<int, KeyboardKey> MyEngine::GLFW::GLFWWindow::glfwKeyMap = {
		{GLFW_KEY_UNKNOWN,       KeyboardKey::KEY_UNKNOWN},
		{GLFW_KEY_SPACE,         KeyboardKey::KEY_SPACE},
		{GLFW_KEY_APOSTROPHE,    KeyboardKey::KEY_APOSTROPHE},
		{GLFW_KEY_COMMA,         KeyboardKey::KEY_COMMA},
		{GLFW_KEY_MINUS,         KeyboardKey::KEY_MINUS},
		{GLFW_KEY_PERIOD,        KeyboardKey::KEY_PERIOD},
		{GLFW_KEY_SLASH,         KeyboardKey::KEY_SLASH},
		{GLFW_KEY_0,             KeyboardKey::KEY_0},
		{GLFW_KEY_1,             KeyboardKey::KEY_1},
		{GLFW_KEY_2,             KeyboardKey::KEY_2},
		{GLFW_KEY_3,             KeyboardKey::KEY_3},
		{GLFW_KEY_4,             KeyboardKey::KEY_4},
		{GLFW_KEY_5,             KeyboardKey::KEY_5},
		{GLFW_KEY_6,             KeyboardKey::KEY_6},
		{GLFW_KEY_7,             KeyboardKey::KEY_7},
		{GLFW_KEY_8,             KeyboardKey::KEY_8},
		{GLFW_KEY_9,             KeyboardKey::KEY_9},
		{GLFW_KEY_SEMICOLON,     KeyboardKey::KEY_SEMICOLON},
		{GLFW_KEY_EQUAL,         KeyboardKey::KEY_EQUAL},
		{GLFW_KEY_A,             KeyboardKey::KEY_A},
		{GLFW_KEY_B,             KeyboardKey::KEY_B},
		{GLFW_KEY_C,             KeyboardKey::KEY_C},
		{GLFW_KEY_D,             KeyboardKey::KEY_D},
		{GLFW_KEY_E,             KeyboardKey::KEY_E},
		{GLFW_KEY_F,             KeyboardKey::KEY_F},
		{GLFW_KEY_G,             KeyboardKey::KEY_G},
		{GLFW_KEY_H,             KeyboardKey::KEY_H},
		{GLFW_KEY_I,             KeyboardKey::KEY_I},
		{GLFW_KEY_J,             KeyboardKey::KEY_J},
		{GLFW_KEY_K,             KeyboardKey::KEY_K},
		{GLFW_KEY_L,             KeyboardKey::KEY_L},
		{GLFW_KEY_M,             KeyboardKey::KEY_M},
		{GLFW_KEY_N,             KeyboardKey::KEY_N},
		{GLFW_KEY_O,             KeyboardKey::KEY_O},
		{GLFW_KEY_P,             KeyboardKey::KEY_P},
		{GLFW_KEY_Q,             KeyboardKey::KEY_Q},
		{GLFW_KEY_R,             KeyboardKey::KEY_R},
		{GLFW_KEY_S,             KeyboardKey::KEY_S},
		{GLFW_KEY_T,             KeyboardKey::KEY_T},
		{GLFW_KEY_U,             KeyboardKey::KEY_U},
		{GLFW_KEY_V,             KeyboardKey::KEY_V},
		{GLFW_KEY_W,             KeyboardKey::KEY_W},
		{GLFW_KEY_X,             KeyboardKey::KEY_X},
		{GLFW_KEY_Y,             KeyboardKey::KEY_Y},
		{GLFW_KEY_Z,             KeyboardKey::KEY_Z},
		{GLFW_KEY_LEFT_BRACKET,  KeyboardKey::KEY_LEFT_BRACKET},
		{GLFW_KEY_BACKSLASH,     KeyboardKey::KEY_BACKSLASH},
		{GLFW_KEY_RIGHT_BRACKET, KeyboardKey::KEY_RIGHT_BRACKET},
		{GLFW_KEY_GRAVE_ACCENT,  KeyboardKey::KEY_GRAVE_ACCENT},
//		{ GLFW_KEY_WORLD_1, KeyboardKey::KEY_WORLD_1 },
//		{ GLFW_KEY_WORLD_2, KeyboardKey::KEY_WORLD_2 },
		{GLFW_KEY_ESCAPE,        KeyboardKey::KEY_ESCAPE},
		{GLFW_KEY_ENTER,         KeyboardKey::KEY_ENTER},
		{GLFW_KEY_TAB,           KeyboardKey::KEY_TAB},
		{GLFW_KEY_BACKSPACE,     KeyboardKey::KEY_BACKSPACE},
		{GLFW_KEY_INSERT,        KeyboardKey::KEY_INSERT},
		{GLFW_KEY_DELETE,        KeyboardKey::KEY_DELETE},
		{GLFW_KEY_RIGHT,         KeyboardKey::KEY_RIGHT},
		{GLFW_KEY_LEFT,          KeyboardKey::KEY_LEFT},
		{GLFW_KEY_DOWN,          KeyboardKey::KEY_DOWN},
		{GLFW_KEY_UP,            KeyboardKey::KEY_UP},
		{GLFW_KEY_PAGE_UP,       KeyboardKey::KEY_PAGE_UP},
		{GLFW_KEY_PAGE_DOWN,     KeyboardKey::KEY_PAGE_DOWN},
		{GLFW_KEY_HOME,          KeyboardKey::KEY_HOME},
		{GLFW_KEY_END,           KeyboardKey::KEY_END},
		{GLFW_KEY_CAPS_LOCK,     KeyboardKey::KEY_CAPS_LOCK},
		{GLFW_KEY_SCROLL_LOCK,   KeyboardKey::KEY_SCROLL_LOCK},
		{GLFW_KEY_NUM_LOCK,      KeyboardKey::KEY_NUM_LOCK},
		{GLFW_KEY_PRINT_SCREEN,  KeyboardKey::KEY_PRINT_SCREEN},
		{GLFW_KEY_PAUSE,         KeyboardKey::KEY_PAUSE},
		{GLFW_KEY_F1,            KeyboardKey::KEY_F1},
		{GLFW_KEY_F2,            KeyboardKey::KEY_F2},
		{GLFW_KEY_F3,            KeyboardKey::KEY_F3},
		{GLFW_KEY_F4,            KeyboardKey::KEY_F4},
		{GLFW_KEY_F5,            KeyboardKey::KEY_F5},
		{GLFW_KEY_F6,            KeyboardKey::KEY_F6},
		{GLFW_KEY_F7,            KeyboardKey::KEY_F7},
		{GLFW_KEY_F8,            KeyboardKey::KEY_F8},
		{GLFW_KEY_F9,            KeyboardKey::KEY_F9},
		{GLFW_KEY_F10,           KeyboardKey::KEY_F10},
		{GLFW_KEY_F11,           KeyboardKey::KEY_F11},
		{GLFW_KEY_F12,           KeyboardKey::KEY_F12},
		{GLFW_KEY_F13,           KeyboardKey::KEY_F13},
		{GLFW_KEY_F14,           KeyboardKey::KEY_F14},
		{GLFW_KEY_F15,           KeyboardKey::KEY_F15},
		{GLFW_KEY_F16,           KeyboardKey::KEY_F16},
		{GLFW_KEY_F17,           KeyboardKey::KEY_F17},
		{GLFW_KEY_F18,           KeyboardKey::KEY_F18},
		{GLFW_KEY_F19,           KeyboardKey::KEY_F19},
		{GLFW_KEY_F20,           KeyboardKey::KEY_F20},
		{GLFW_KEY_F21,           KeyboardKey::KEY_F21},
		{GLFW_KEY_F22,           KeyboardKey::KEY_F22},
		{GLFW_KEY_F23,           KeyboardKey::KEY_F23},
		{GLFW_KEY_F24,           KeyboardKey::KEY_F24},
		{GLFW_KEY_F25,           KeyboardKey::KEY_F25},
		{GLFW_KEY_KP_0,          KeyboardKey::KEY_KP_0},
		{GLFW_KEY_KP_1,          KeyboardKey::KEY_KP_1},
		{GLFW_KEY_KP_2,          KeyboardKey::KEY_KP_2},
		{GLFW_KEY_KP_3,          KeyboardKey::KEY_KP_3},
		{GLFW_KEY_KP_4,          KeyboardKey::KEY_KP_4},
		{GLFW_KEY_KP_5,          KeyboardKey::KEY_KP_5},
		{GLFW_KEY_KP_6,          KeyboardKey::KEY_KP_6},
		{GLFW_KEY_KP_7,          KeyboardKey::KEY_KP_7},
		{GLFW_KEY_KP_8,          KeyboardKey::KEY_KP_8},
		{GLFW_KEY_KP_9,          KeyboardKey::KEY_KP_9},
		{GLFW_KEY_KP_DECIMAL,    KeyboardKey::KEY_KP_DECIMAL},
		{GLFW_KEY_KP_DIVIDE,     KeyboardKey::KEY_KP_DIVIDE},
		{GLFW_KEY_KP_MULTIPLY,   KeyboardKey::KEY_KP_MULTIPLY},
		{GLFW_KEY_KP_SUBTRACT,   KeyboardKey::KEY_KP_SUBTRACT},
		{GLFW_KEY_KP_ADD,        KeyboardKey::KEY_KP_ADD},
		{GLFW_KEY_KP_ENTER,      KeyboardKey::KEY_KP_ENTER},
		{GLFW_KEY_KP_EQUAL,      KeyboardKey::KEY_KP_EQUAL},
		{GLFW_KEY_LEFT_SHIFT,    KeyboardKey::KEY_LEFT_SHIFT},
		{GLFW_KEY_LEFT_CONTROL,  KeyboardKey::KEY_LEFT_CONTROL},
		{GLFW_KEY_LEFT_ALT,      KeyboardKey::KEY_LEFT_ALT},
		{GLFW_KEY_LEFT_SUPER,    KeyboardKey::KEY_LEFT_SUPER},
		{GLFW_KEY_RIGHT_SHIFT,   KeyboardKey::KEY_RIGHT_SHIFT},
		{GLFW_KEY_RIGHT_CONTROL, KeyboardKey::KEY_RIGHT_CONTROL},
		{GLFW_KEY_RIGHT_ALT,     KeyboardKey::KEY_RIGHT_ALT},
		{GLFW_KEY_RIGHT_SUPER,   KeyboardKey::KEY_RIGHT_SUPER},
//		{ GLFW_KEY_MENU, KeyboardKey::KEY_MENU },
};

extern "C" {
	MyEngine::Plugin* exportMyEngineGLFWPlugin() {
		return new GLFW::GLFWWindowPlugin();
	}

	void cleanupMyEngineGLFWPlugin(MyEngine::Plugin* plugin) {
		delete plugin;
	}
}
